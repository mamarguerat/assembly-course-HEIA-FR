/**
  ****************************************************************************
  * @file    HAL_MSP430.c
  * @author  Scherwey Roland
  * @date    25.04.2015
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
  // a)
}

/// @brief  This method initializes the GPIO's - also check Timer usage
/// @param  -
void initGPIO(void)
{
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure GPIO
  PAOUT = PBOUT = PJOUT = 0;
  PADIR = PBDIR = 0;

  P4DIR &= ~BIT5; // P4.5 (S1) input (is already configured as input after reset...)
  P4REN |=  BIT5; // Enable pull-up/pull-down on P4.5 (S1)
  P4OUT |=  BIT5; // Configure pull-up on P4.5 (S1) (see MSP430FR5969 schematics)

  // LED handling
  P1DIR |= BIT0;        // P1.0 output
  P4DIR |= BIT6;        // P4.6 output
}

/// @brief  This method initializes the timer A for blinking the LED
/// @param  -
void initTimer(void)
{
  // b)

  // c)

}


//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  initTimer();// Initializes Timer
}


