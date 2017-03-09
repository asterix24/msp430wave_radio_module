// *************************************************************************************************
// Global project defines, variables and functions
// *************************************************************************************************

#ifndef __PROJECT_H
#define __PROJECT_H

// *************************************************************************************************
// Include section

// *************************************************************************************************
// Defines section

// MCLK frequency in Hz the MCU is running on (only used for timing calculations)
#define MCLK_FREQUENCY                 (16000000)

// Init activity signal
#define LED_INIT()      do { P1OUT |=  (BIT0); P1DIR |=  (BIT0); } while (0)
#define LED_TOGGLE()    do { P1OUT ^=  (BIT0); } while(0)
#define LED_ON()        do { P1OUT |=  (BIT0); } while(0)
#define LED_OFF()       do { P1OUT &= ~(BIT0); } while(0)

#define LED_PINS        (GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4)
#define LED_PORT        GPIO_PORT_P1

#endif // __PROJECT_H
// End of file
