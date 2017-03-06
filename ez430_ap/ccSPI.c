// *************************************************************************************************
//  Hardware SPI interface to TI's CCxx00 radios. Supports writing bytes and words.
// *************************************************************************************************


// *************************************************************************************************
// Include section

#include "project.h"
#include "ccSPI.h"
#include "ccxx00.h"
#include "driverlib.h"


// *************************************************************************************************
// Defines section

// CSN pin to high
#define CC_CSN_HIGH                    {CC_CSN_OUT |=  CC_CSN_PIN;}
// CSN pin to low
#define CC_CSN_LOW                     {CC_CSN_OUT &= ~CC_CSN_PIN;}
// Initialize CSN pin
#define CC_CSN_INIT                    {CC_CSN_DIR |=  CC_CSN_PIN;}

// SDI pin level read out
#define SDI_LEVEL                      (CC_SDI_IN & CC_SDI_PIN)


// *************************************************************************************************
// Function prototype section


// *************************************************************************************************
// Implementation

// *************************************************************************************************
// Setup SPI pins.
// *************************************************************************************************
void CC_SPI_Init_v(void)
{
  // Initialize SPI interface to radio
  CC_SPI_CTL0 |= UCSYNC | UCMST | UCMSB // SPI master, 8 data bits,  MSB first,
                 | UCCKPH;              //  clock idle low, data output on falling edge
  CC_SPI_CTL1 |= UCSSEL1;               // SMCLK as clock source
  CC_SPI_BR0   = CC_BR_DIVIDER;         // Low byte of division factor for baud rate
  CC_SPI_BR1   = 0x00;                  // High byte of division factor for baud rate
  CC_SPI_CTL1 &= ~UCSWRST;              // Start SPI hardware
  
  // Pins to SPI function
  CC_SDO_SEL |= CC_SDO_PIN;
  CC_SDI_SEL |= CC_SDI_PIN;
  CC_SCK_SEL |= CC_SCK_PIN;
  CC_SDO_DIR |= CC_SDO_PIN;
  CC_SCK_DIR |= CC_SCK_PIN;
  
  // Init CSN and strobe it low/high
  CC_CSN_HIGH;
  CC_CSN_INIT;
  __delay_cycles(30ul * (MCLK_FREQUENCY / 1000000)); // 30us at selected CPU clock speed
  CC_CSN_LOW;
  __delay_cycles(30ul * (MCLK_FREQUENCY / 1000000)); // 30us at selected CPU clock speed
  CC_CSN_HIGH;
  __delay_cycles(45ul * (MCLK_FREQUENCY / 1000000)); // 45us at selected CPU clock speed

  // Select radio
  CC_CSN_LOW;
  // Wait for SDI indicating chip ready
  while (SDI_LEVEL);
  // Send SRES strobe
  CC_TX_BUFFER = CCXX00_STROBE_SRES;
  // Wait until XOSC and voltage regulator stabilized
  while (SDI_LEVEL);
  __delay_cycles(100ul * (MCLK_FREQUENCY / 1000000)); // 100us at selected CPU clock speed
}

// *************************************************************************************************
// Select chip connected to the SPI.
// *************************************************************************************************
void CC_SPI_SelectChip_v(void)
{
  // Select radio
  CC_CSN_LOW;
}

// *************************************************************************************************
// Select chip connected to the SPI and wait until chip ready.
// *************************************************************************************************
void CC_SPI_SelectChipWaitUntilReady_v(void)
{
  // Select radio
  CC_CSN_LOW;
  // Wait for SDI indicating chip ready
  while (SDI_LEVEL);
}

// *************************************************************************************************
// Deselect chip connected to the SPI.
// *************************************************************************************************
void CC_SPI_DeselectChip_v(void)
{
  // Deselect radio
  CC_CSN_HIGH;
}

// *************************************************************************************************
// Write one byte to and read one byte from the SPI in burst mode, chip select has to be handled
// manually
// *************************************************************************************************
uint8_t CC_SPI_WriteRead_u8(uint8_t Data_u8)
{
  // Write byte to be sent to TX buffer
  CC_TX_BUFFER = Data_u8;
  // Wait until data transmitted and received
  while (CC_STATUS & UCBUSY);
  // Return received data
  return CC_RX_BUFFER;
}

// *************************************************************************************************
// Write one byte to and read one byte from the SPI.
// *************************************************************************************************
uint8_t CC_SPI_SingleWriteRead_u8(uint8_t Data_u8)
{
  uint8_t Result_u8;

  CC_SPI_SelectChip_v();
  Result_u8 = CC_SPI_WriteRead_u8(Data_u8);
  CC_SPI_DeselectChip_v();

  return Result_u8;
}


// *************************************************************************************************
// End of file
