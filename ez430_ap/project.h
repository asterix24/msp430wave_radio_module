// *************************************************************************************************
// Global project defines, variables and functions
// *************************************************************************************************

#ifndef __PROJECT_H
#define __PROJECT_H

// *************************************************************************************************
// Include section

#include <msp430F5509.h>

// *************************************************************************************************
// Defines section

// MCLK frequency in Hz the MCU is running on (only used for timing calculations)  
#define MCLK_FREQUENCY                 (16000000)

// Init RF activity signal
#define INIT_TX_ACTIVITY               { }
// Switch RF activity signal on
#define TX_ACTIVITY_ON                 { P1OUT |=  (BIT0); }
// Switch RF activity signal off
#define TX_ACTIVITY_OFF                { P1OUT &= ~(BIT0); }
// Toggle RF activity signal
#define TX_ACTIVITY_TOGGLE             { P1OUT ^=  (BIT0); }

// Init activity signal
#define INIT_RX_ACTIVITY               { P1OUT |=  (BIT0);                                           \
                                         P1DIR |=  (BIT0); }
//#define INIT_RX_ACTIVITY               {  }
// Switch activity signal on
#define RX_ACTIVITY_ON                 { P1OUT |=  (BIT0); }
// Switch activity signal off
#define RX_ACTIVITY_OFF                { P1OUT &= ~(BIT0); }
// Toggle activity signal
#define RX_ACTIVITY_TOGGLE             { P1OUT ^=  (BIT0); }

#define LED_ON                         { P1OUT |=  (BIT0); }
#define LED_OFF                        { P1OUT &= ~(BIT0); }

// Product ID
#define PRODUCT_ID                     (0x23456789)

// BlueRobin TX serial
#define TX_SERIAL_NO                   (1234567u)
#define BRTX_PROFILE                   (33)

// *************************************************************************************************
// API section

// *************************************************************************************************
#endif // __PROJECT_H
// End of file
