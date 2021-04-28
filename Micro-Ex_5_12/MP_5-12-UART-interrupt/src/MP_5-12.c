/**
  ****************************************************************************
  * @file    MP_5-12.c
  * @author  Scherwey Roland
  * @date    20.04.2015
  * @brief   Initialization and use of UART interface
  *          Loopback received characters directly in ISR
  *
  *          Following ULP's deactivated: 1.1, 2.1, 3.1, 5.1, 5.3
  *

              MSP430FR5969
         -----------------
        |              XIN|-
        |                 |  32KHz Crystal
      ->|RST              |
        |                 |
        |     P2.0/UCA0TXD|----   -
        |                 |       --<> Debug UART
        |                 |       -
        |     P2.1/UCA0RXD|----   -
        |                 |
  ****************************************************************************/

#include <msp430.h>

#include "HAL\HAL_MSP430.h"


//----------------------------------------------------------------------------
// Local functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
  initChip();

#warn c) Enable masquable interrupts

  while(1)
  {
    P1OUT ^= BIT0;      // Toggles P1.0 (LED2) every second
    P4OUT &= ~BIT6;     // Reset   P4.6 (LED1) to measure ISR latency
    __delay_cycles(8000000);
  }
}

#warn b) add UART ISR handler

// USCI_A0 interrupt vector handler
 void USCI_A0_ISR(void)
{
}
