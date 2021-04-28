/**
  ****************************************************************************
  * @file    MP_5-21.c
  * @author  Scherwey Roland
  * @date    24.04.2015
  * @brief   Blink LED's with Timer
  *
  *          Following ULP's deactivated: 1.1, 2.1, 3.1, 5.1, 5.3
  *

            MSP430FR5969
         -----------------
      ->|RST              |
        |                 |
        |            P1.0 |-->LED2 blink @2Hz
        |            P4.6 |-->LED1 blink @0.5Hz
        |                 |
         -----------------

  ****************************************************************************/

#include "HAL/HAL_MSP430.h"

#include <msp430.h>


//----------------------------------------------------------------------------
// Local functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
  initChip();

  __bis_SR_register(GIE | LPM4_bits);     // enable interrupts and enable LPM4
  __no_operation();                       // Remain in LPM4

  for (;;);
};

// Timer_A0 TACCR0 interrupt vector handler
#pragma vector= TIMER0_A0_VECTOR
__interrupt void timerA0_ISR(void)
{
   P1OUT ^= BIT0;      // Toggles P1.0 (LED2) every 250ms -> 2Hz
}

// d)