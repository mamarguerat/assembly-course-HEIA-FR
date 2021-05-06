/**
  ****************************************************************************
  * @file    HAL_MSP430.c
  * @author  Scherwey Roland & Marguerat Martin
  * @date    06.05.2021
  * @version 1.1
  * @brief   Implementation of MSP430 HAL (Hardware Abstraction Layer)
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "HAL_MSP430.h"


#include <msp430.h>


//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

/// @brief  This method initializes the Clock System
/// @param  -
void initCS(void)
{
  // Configure one FRAM waitstate as required by the device datasheet for MCLK
  // operation beyond 8MHz _before_ configuring the clock system.
  FRCTL0 = FRCTLPW | NWAITS_0;

  CSCTL0_H = CSKEY_H;                       // Unlock CS registers

  //CSCTL1   = DCOFSEL_0;                     // Set DCO to 1MHz
  CSCTL2   = SELA__VLOCLK;  // Set ACLK=VLOCLK and SMCLK = MCLK = DCO
  //CSCTL3   = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 (register cleared to 0...)

  CSCTL0_H = 0;                             // Lock CS registers

}

/// @brief  This method initializes the GPIO's - also check Timer usage
/// @param  -
void initGPIO(void)
{
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                             // to activate previously configured port settings

    // Configure GPIO
    P1OUT &= ~0x01;                 // Clear P1.0 output latch for a defined power-on state
    P1SEL0 |= 0x01;                 // Set P1.0 to use Timer A0
    P1SEL1 &= ~0x01;
    P1DIR |= 0x01;                  // Set P1.0 to output direction
    P2SEL0 |= BIT0;                 // Output ACLK on P2.0
    P2SEL1 |= BIT0;
    P2DIR |= BIT0;
}

void initTimer(void)
{
  TA0CCTL0  &= ~CCIE;             // TA0CCR0 interrupt disabled

  TA0CTL   =  MC__STOP            // Stop timer first...
           |  TACLR;              // ... and clear it

  TA0CTL &= ~0x200;               // Use ACLK as clock
  TA0CTL |= 0x100;
  TA0CTL &= ~0x20;                // Set to UP mode
  TA0CTL |= 0x10;
  TA0CCTL1 &= ~0x80;              // Set output mode to Set/Reset
  TA0CCTL1 |= 0x60;
  TA0CCR0 = 5000;                 // Set maximum time to 500ms
  TA0CCR1 = 2500;                 // Set half-time to 250ms (duty cycle 50%)
}


//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  initTimer(); // used for periodically wake-up from Low Power

}


