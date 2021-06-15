/**
  ****************************************************************************
  * @file    TP7_main.c
  * @author  Yann Kurzo
  * @date    15 mai 2015
  * @brief   The goal is to check the power usage between the different
  *          powering modes.
  *          If BUTTON_MODE_CHANGE is defined to 1, the right button S2 can be
  *          used to switch between staying in an active loop, LPM0, or LPM3.
  *          The program stays automatically 2.5 seconds in an active loop,
  *          and 2.5 seconds in one of the 3 modes mentioned earlier.
  *          If BUTTON_MODE_CHANGE is not defined to 1, the program changes
  *          the mode automatically approximately every 10 seconds.
  * @note    To avoid measuring wrong values, all the jumpers on J13 should
  *          be removed. Furthermore, the jumper J9 should also be removed to
  *          avoid back currents on the supercap. To measure the current
  *          consumption, the best way to supply is external power with the J12
  *          connector and add an amperemeter in serial with the power supply.
  *          Typical values are (Vcc = 3.3V):
  *            Active loop: 480 [uA] (~1.58mW @3.3V)
  *            LPM0       : 184 [uA] (~0.61mW @3.3V)
  *            LPM3       : 22  [uA] (~0.07mW @3.3V)
  * @note    There is no debounce on the right button. It is then possible
  *          to miss a mode when clicking.
  *

              MSP430FR5969
         -----------------
        |                 |
        |                 |
      ->|RST              |
        |                 |
        |                 |
         -----------------

//  ****************************************************************************/

#include "HAL/HAL_MSP430.h"

#include <msp430.h>
#include <stdint.h>

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

#define BUTTON_MODE_CHANGE 0

typedef enum
{
  eTESTMODE_ACTIVE_LOOP,  // Wait 2.5 second in a loop
  eTESTMODE_LPM0,         // Wait 2.5 second in low power mode 0, wake up by TimerA0
  eTESTMODE_LPM3,         // Wait 2.5 second in low power mode 3, wake up by TimerA0
  eNbrOfTestModes
}testMode_t;

static testMode_t testMode = eTESTMODE_ACTIVE_LOOP;

enum
{
  eCYCLES_IN_ACTIVE_MODE = (8000000 / 10)   // ==> 100ms @ 8MHz
};

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------

int main(void)
{
  // Init chip
  initChip();

  // Enable interrupts
  __bis_SR_register(GIE);

  // Main loop
  while(1)
  {
#warn a Wait in active mode during 100ms


#warn b Enter in LPM0 or LPM3, do not forget the nop operation
    switch (testMode)
    {
      case eTESTMODE_LPM0:
              // Enter LPM0 and remain in LPM0
              // Wake up by timer every 5 seconds
             break;
      case eTESTMODE_LPM3:
              // Enter LPM3 and remain in LPM3
              // Wake up by timer every 5 seconds
             break;
      default:
             break;
    }
  }
}

// Timer_A TACCR0 interrupt vector handler at 0.2Hz
#pragma vector= TIMER0_A0_VECTOR
__interrupt void timerA0_ISR(void)
{
#if(BUTTON_MODE_CHANGE != 1)
  // Update counter to change every 10 seconds
  static uint16_t cnt = 0;
  if(++cnt == 2)
  {
    cnt = 0;

    // Update automatically testMode
    if(++testMode == eNbrOfTestModes)
    {
      testMode = eTESTMODE_ACTIVE_LOOP;
    }
  }
#endif
  #warn c Exit the right mode depending on the actual testMode
  // Exit right mode after interrupt
  switch (testMode)
  {
    case eTESTMODE_LPM0:
           break;
    case eTESTMODE_LPM3:
           break;
    default:
           break;
  }
}


#if(BUTTON_MODE_CHANGE == 1)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  switch(__even_in_range(P1IV, P1IV_P1IFG7))
  {
    // Right button
    case P1IV_P1IFG1:
      // Activate LED while it is pressed
      P1OUT |= BIT0;
      while(0 == (P1IN & BIT1));

      // Shut down LED
      P1OUT &= ~BIT0;

      // Update testMode
      switch(testMode)
      {
        case eTESTMODE_ACTIVE_LOOP:
          ++testMode;
          break;
        case eTESTMODE_LPM0:
          _bic_SR_register_on_exit(LPM0_bits);
          ++testMode;
          break;
        case eTESTMODE_LPM3:
          _bic_SR_register_on_exit(LPM3_bits);
          testMode = eTESTMODE_ACTIVE_LOOP;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
#endif


// Default / unused trap isr assignation - put all unused ISR vector here
#pragma vector = RTC_VECTOR, PORT4_VECTOR, PORT3_VECTOR, TIMER3_A1_VECTOR, TIMER3_A0_VECTOR, \
    PORT2_VECTOR, TIMER2_A1_VECTOR, TIMER2_A0_VECTOR, /*PORT1_VECTOR,*/ TIMER1_A1_VECTOR,    \
    TIMER1_A0_VECTOR, DMA_VECTOR, USCI_A1_VECTOR, /*TIMER0_A0_VECTOR,*/ \
    ADC12_VECTOR, USCI_B0_VECTOR, USCI_A0_VECTOR, WDT_VECTOR, \
    TIMER0_B1_VECTOR, TIMER0_B0_VECTOR, COMP_E_VECTOR, UNMI_VECTOR, SYSNMI_VECTOR
__interrupt void defaultUnusedIsr(void)
{
  // this is a default trap ISR - check for the interrupt cause here by
  // checking the interrupt flags...
  for(;;);
}
