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

  CSCTL1   = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
  CSCTL2   = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set ACLK=VLOCLK and SMCLK = MCLK = DCO
  CSCTL3   = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 (register cleared to 0...)

  CSCTL0_H = 0;                             // Lock CS registers

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

  // UART handling
  P2SEL1 |=   BIT0 | BIT1;        // USCI_A0 UART operation
  P2SEL0 &= ~(BIT0 | BIT1);       // ""
  P1DIR |= 0x01;                  // P1.0 output

  // LED handling
  P1DIR |= BIT0;        // P1.0 output (LED2)
  P4DIR |= BIT6;        // P4.6 output (LED1)
}

/// @brief  This method initializes the UART
///         Baud Rate calculation
///     8000000/(16*115200) = 4.340
///     Fractional portion = 0.340
///     User's Guide Table 21-5: UCBRx  = 0x04
///                              UCBRFx = 5
///                              UCBRSx = 0x55
void init_UART(void)
{
  // Configure USCI_A0 for UART mode
  UCA0CTLW0  = UCSWRST;                    // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;              // CLK = SMCLK
  UCA0BRW = 4;                             // 8000000/16/9600
  UCA0MCTLW |= (0x55 << 8)                 // UCBRSx = 0x55
            |  (   5 << 4)                 // UCBRFx = 5
            |  UCOS16;
  UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI

#warn a) enable necessary UART Interrupts
 
}

//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  init_UART(); // USCI_A0 for UART mode
}


