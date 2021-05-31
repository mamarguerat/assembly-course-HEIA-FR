/**
  ****************************************************************************
  * @file    HAL_MSP430.c
  * @author  Bullot Dominique
  * @date   17.05.2017
  * @brief   Implementation of MSP430 HAL (Hardware Abstraction Layer) for TP6
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "HAL_MSP430.h"
#include "HAL_SPI.h"

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

  // SPI handling
  enableSPI_MOSI(false);     // necessary as TC77 has bidirectional SDIO

//#warn 2) Implement SPI GPIO initialization

  P1SEL1 |= BIT7;   // Configure MISO  P1.7
  P1SEL0 &=~BIT7;
  P2SEL1 |= BIT2;   // Configure clk   P2.2
  P2SEL0 &=~BIT2;
  P1DIR  |= BIT3;   // P1.3 manual CS (active low) for MCP23S08/TC77 initially inactive (high)
  P1OUT  |= BIT3;
  P1SEL1 &=~BIT3;
  P1SEL0 &=~BIT3;

    // LED handling
  P1DIR  |= BIT0;   // P1.0 output
  P1SEL1 &=~BIT0;
  P1SEL0 &=~BIT0;
  P4DIR  |= BIT6;   // P4.6 output
  P4SEL1 &=~BIT6;
  P4SEL0 &=~BIT6;

}


//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  spi_init(); // configure SPI
}


