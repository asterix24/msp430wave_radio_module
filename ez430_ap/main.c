/**
 * \file
 * <!--
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Copyright 2017 Daniele Basile <asterix24@gmail.com>
 *
 * -->
 *
 * \brief EZ430 Access point.
 *
 * \author Daniele Basile <asterix24@gmail.com>
 *
 */

#include <string.h>
#include "driverlib.h"
#include "ccSPI.h"
#include "ccxx00.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"                     //USB-specific functions
#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "USB_app/usbConstructs.h"

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"

// Function declarations
void convertTimeBinToASCII(uint8_t* timeStr);
void initRTC(void);

// Application globals
volatile uint8_t hour = 4, min = 30, sec = 00;  // Real-time clock (RTC) values.  4:30:00
volatile uint8_t bSendTimeToHost = FALSE;       // RTC-->main():  "send the time over USB"
uint8_t timeStr[9];                    // Stores the time as an ASCII string


void InitClock_v(void)
{
  // Enable 32kHz ACLK on XT1 via external crystal and 26MHz on XT2 via external clock signal
  P5SEL |=  (BIT2 | BIT3 | BIT4 | BIT5); // Select XINs and XOUTs
//  UCSCTL6 &= ~XT1OFF;        // Switch on XT1, keep highest drive strength - default
//  UCSCTL6 |=  XCAP_3;        // Set internal load caps to 12pF - default
//  UCSCTL4 |=  SELA__XT1CLK;  // Select XT1 as ACLK - default

  // Configure clock system
  _BIS_SR(SCG0);    // Disable FLL control loop
  UCSCTL0 = 0x0000; // Set lowest DCOx, MODx to avoid temporary overclocking
  // Select suitable DCO frequency range and keep modulation enabled
  UCSCTL1 = DCORSEL_5; // DCO frequency above 8MHz but not bigger than 16MHz
  UCSCTL2 = FLLD__2 | (((MCLK_FREQUENCY + 0x4000) / 0x8000) - 1); // Set FLL loop divider to 2 and
                                                                  // required DCO multiplier
//  UCSCTL3 |= SELREF__XT1CLK;                    // Select XT1 as FLL reference - default
//  UCSCTL4 |= SELS__DCOCLKDIV | SELM__DCOCLKDIV; // Select XT1 as ACLK and
                                                  // divided DCO for SMCLK and MCLK - default
  _BIC_SR(SCG0);                                  // Enable FLL control loop again

  // Loop until XT1 and DCO fault flags are reset
  do
  {
    // Clear fault flags
    UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
    SFRIFG1 &= ~OFIFG;
  } while ((SFRIFG1 & OFIFG));

  // Worst-case settling time for the DCO when changing the DCO range bits is:
  // 32 x 32 x MCLK / ACLK
  __delay_cycles(((32 * MCLK_FREQUENCY) / 0x8000) * 32);

  _BIS_SR(SCG0);    // Disable FLL control loop
}


void InitPorts_v(void)
{
  // Initialize all unused pins as low level output
  P4OUT  &= ~(                            BIT4 | BIT5 | BIT6 | BIT7);
  P5OUT  &= ~(BIT0 | BIT1                                          );
  P6OUT  &= ~(BIT0 | BIT1 | BIT2 | BIT3                            );
  P4DIR  |=  (                            BIT4 | BIT5 | BIT6 | BIT7);
  P5DIR  |=  (BIT0 | BIT1                                          );
  P6DIR  |=  (BIT0 | BIT1 | BIT2 | BIT3                            );
}

/*
 * ======== main ========
 */
void main(void)
{
    WDT_A_hold(WDT_A_BASE); //Stop watchdog timer
    P5SEL |= BIT4 | BIT5;
    // As fast as possible 26MHz to GDO2 pin of CC1101 as it is required to clock USB
    CC_SPI_Init_v();
    //CC_SPI_SelectChipWaitUntilReady_v();
    CC_SPI_WriteRead_u8(CCXX00_REG_IOCFG0);
    CC_SPI_WriteRead_u8(0x2E);
    CC_SPI_WriteRead_u8(CCXX00_REG_IOCFG2);
    CC_SPI_WriteRead_u8(0x30);
    CC_SPI_DeselectChip_v();
    UCSCTL6 |= XT2BYPASS;

    // Initialize unused port pins
    InitPorts_v();
    INIT_RX_ACTIVITY;
    LED_ON;

    // Set Vcore to the level required for USB operation
    PMM_setVCore(PMM_CORE_LEVEL_3);
    // Initialize clock system
    InitClock_v();
    // Initialize debug output pins
    INIT_TX_ACTIVITY;

        // Initialize the USB module, and connect to the USB host (if one is present)
     USB_setup(TRUE, TRUE);
     LED_OFF;
    __enable_interrupt();    // Enable general interrupts
    while (1) {
        // Enter LPM0, which keeps the DCO/FLL active but shuts off the
        // CPU.  For USB, you can't go below LPM0!
       // __bis_SR_register(LPM0_bits + GIE);
            if (USBCDC_sendDataInBackground("Hello!\n", sizeof("Hello!\n"), CDC0_INTFNUM, 1000))
            {
              _NOP();  // If it fails, it'll end up here.  Could happen if
                       // the cable was detached after the connectionState()
            }           // check, or if somehow the retries failed
       }  //while(1)
}  //main()


// Starts a real-time clock on TimerA_0.  Earlier we assigned ACLK to be driven
// by the REFO, at 32768Hz.  So below we set the timer to count up to 32768 and
// roll over; and generate an interrupt when it rolls over.
void initRTC(void)
{
    TA0CCR0 = 32768;
    TA0CTL = TASSEL_1+MC_1+TACLR; // ACLK, count to CCR0 then roll, clear TAR
    TA0CCTL0 = CCIE;              // Gen int at rollover (TIMER0_A0 vector)
}


// Timer0 A0 interrupt service routine.  Generated when TimerA_0 (real-time
// clock) rolls over from 32768 to 0, every second.
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not found!
#endif
{
    if (sec++ == 60)
    {
        sec = 0;
        if (min++ == 60)
        {
            min = 0;
            if (hour++ == 24)
            {
                hour = 0;
            }
        }
    }

    bSendTimeToHost = TRUE;                 // Time to update
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM
}


// Convert a number 'bin' of value 0-99 into its ASCII equivalent.  Assumes
// str is a two-byte array.
void convertTwoDigBinToASCII(uint8_t bin, uint8_t* str)
{
    str[0] = '0';
    if (bin >= 10)
    {
        str[0] = (bin / 10) + 48;
    }
    str[1] = (bin % 10) + 48;
}


// Convert the binary globals hour/min/sec into a string, of format "hr:mn:sc"
// Assumes str is an nine-byte string.
void convertTimeBinToASCII(uint8_t* str)
{
    uint8_t hourStr[2], minStr[2], secStr[2];

    convertTwoDigBinToASCII(hour, hourStr);
    convertTwoDigBinToASCII(min, minStr);
    convertTwoDigBinToASCII(sec, secStr);

    str[0] = hourStr[0];
    str[1] = hourStr[1];
    str[2] = ':';
    str[3] = minStr[0];
    str[4] = minStr[1];
    str[5] = ':';
    str[6] = secStr[0];
    str[7] = secStr[1];
    str[8] = '\n';
}


/*
 * ======== UNMI_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not found!
#endif
{
        switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG )) {
        case SYSUNIV_NONE:
                __no_operation();
                break;
        case SYSUNIV_NMIIFG:
                __no_operation();
                break;
        case SYSUNIV_OFIFG:

                UCS_clearFaultFlag(UCS_XT2OFFG);
                UCS_clearFaultFlag(UCS_DCOFFG);
                SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
                break;
        case SYSUNIV_ACCVIFG:
                __no_operation();
                break;
        case SYSUNIV_BUSIFG:
                // If the CPU accesses USB memory while the USB module is
                // suspended, a "bus error" can occur.  This generates an NMI.  If
                // USB is automatically disconnecting in your software, set a
                // breakpoint here and see if execution hits it.  See the
                // Programmer's Guide for more information.
                SYSBERRIV = 0;  // Clear bus error flag
                USB_disable();  // Disable
        }
}

//Released_Version_5_10_00_17
