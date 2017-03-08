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

// Init activity signal
#define LED_INIT()      do { P1OUT |=  (BIT0); P1DIR |=  (BIT0); } while (0)
#define LED_TOGGLE()    do { P1OUT ^=  (BIT0); } while(0)
#define LED_ON()        do { P1OUT |=  (BIT0); } while(0)
#define LED_OFF()       do { P1OUT &= ~(BIT0); } while(0)

#endif // __PROJECT_H
// End of file
