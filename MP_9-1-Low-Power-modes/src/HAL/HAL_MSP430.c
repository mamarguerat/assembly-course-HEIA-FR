/**
  ****************************************************************************
  * @file    HAL_MSP430.c
  * @author  Scherwey Roland
  * @date    08.04.2015
  * @brief   Implementation of MSP430 HAL (Hardware Abstraction Layer) for TP5
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------

#include "HAL/HAL_MSP430.h"

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

  CSCTL0_H = CSKEY_H;               // Unlock CS registers

  CSCTL1   = DCOFSEL_3 | DCORSEL;   // Set DCO to 8MHz
                                    // Set VLOCLK to ~10kHz (default)

  CSCTL2   = SELA__VLOCLK |         // Set ACLK = VLOCLK
             SELS__DCOCLK |         // Set SMCLK = DCO
             SELM__DCOCLK;          // Set MCLK = DC0

  CSCTL3   = DIVA__1 |              // Set ACLK / 1 = 10kHz
             DIVS__1 |              // Set SMCLK / 1 = 8MHz
             DIVM__1;               // Set MCLK / 1 = 8MHz

  CSCTL0_H = 0;                     // Lock CS registers
}

/// @brief  This method initializes the GPIO's - also check Timer usage
/// @param  -
void initGPIO(void)
{
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Output at 0
  PAOUT = 0;
  PBOUT = 0;
  PJOUT = 0;

  // Configure as output for low power consumption
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
  P4DIR = 0xFF;

  // Configure button right as input with pull-up and interrupt
  P1DIR &= ~BIT1;
  P1OUT |= BIT1;
  P1REN |= BIT1;

  P1IFG &= ~BIT1;
  P1IES |= BIT1;
  P1IE |= BIT1;
}

/// @brief  This method initializes the TimerA0 for 1s operation with interrupts
void initTimerA0()
{
  // Configure TimerA0
  TA0CTL   |= MC__STOP;           // Stop timer first
  TA0CTL   |= TASSEL__ACLK        // ACLK as clock source 10kHz
           |  ID__1               // divide by 1
           |  MC__UP              // mode up
           |  TACLR;              // clear TAR

  TA0CCR0   = 50000 - 1;          // fréquence = 10E3/50E3 = 2E-1 = 0.2 [Hz]
  TA0CCTL0  = CCIE;               // TA0CCR0 interrupt enabled
}

//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD | WDTSSEL_1; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  initTimerA0();  // used for periodically wake-up from Low Power
}
