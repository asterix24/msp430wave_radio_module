// *************************************************************************************************
// Public header for hardware SPI to TI's CCxx00 radios.
// *************************************************************************************************

#ifndef __CC_SPI_H
#define __CC_SPI_H


// *************************************************************************************************
// Include section

#include "project.h"
#include "driverlib.h"

#ifdef __cplusplus
extern "C" {
#endif


// *************************************************************************************************
// Defines section

// DCO frequency division factor determining speed of the radio SPI interface in Hz
//  Max. value for CC1101 is 6.5MHz
#define CC_BR_DIVIDER        ((MCLK_FREQUENCY / 6500000) + 1)

// Required delay after selecting radio in us
#define CC_CSN_DELAY         (20)

// Port and pin resources for SPI interface to acceleration sensor
// SPI registers
#define CC_TX_BUFFER         (UCB1TXBUF)
#define CC_RX_BUFFER         (UCB1RXBUF)
#define CC_STATUS            (UCB1STAT)
#define CC_TX_IFG            (UCB1TXIFG)
#define CC_RX_IFG            (UCB1RXIFG)
#define CC_IRQ_REG           (UCB1IFG)
#define CC_SPI_CTL0          (UCB1CTL0)
#define CC_SPI_CTL1          (UCB1CTL1)
#define CC_SPI_BR0           (UCB1BR0)
#define CC_SPI_BR1           (UCB1BR1)
// Port and pin resource for SDO
#define CC_SDO_OUT           (P4OUT)
#define CC_SDO_DIR           (P4DIR)
#define CC_SDO_SEL           (P4SEL)
#define CC_SDO_PIN           (BIT1)
// Port and pin resource for SDI
#define CC_SDI_IN            (P4IN)
#define CC_SDI_OUT           (P4OUT)
#define CC_SDI_SEL           (P4SEL)
#define CC_SDI_PIN           (BIT2)
// Port and pin resource for SCK
#define CC_SCK_OUT           (P4OUT)
#define CC_SCK_DIR           (P4DIR)
#define CC_SCK_SEL           (P4SEL)
#define CC_SCK_PIN           (BIT3)
// Port and pin resource for chip select
#define CC_CSN_OUT           (P4OUT)
#define CC_CSN_DIR           (P4DIR)
#define CC_CSN_PIN           (BIT0)


// *************************************************************************************************
// Typedef section


// *************************************************************************************************
// API section

void CC_SPI_Init_v(void);
void CC_SPI_SelectChip_v(void);
void CC_SPI_SelectChipWaitUntilReady_v(void);
void CC_SPI_DeselectChip_v(void);
uint8_t   CC_SPI_WriteRead_u8(uint8_t Data_u8);
uint8_t   CC_SPI_SingleWriteRead_u8(uint8_t Data_u8);


#ifdef __cplusplus
}
#endif

// *************************************************************************************************
#endif // __CC_SPI_H
// End of file
