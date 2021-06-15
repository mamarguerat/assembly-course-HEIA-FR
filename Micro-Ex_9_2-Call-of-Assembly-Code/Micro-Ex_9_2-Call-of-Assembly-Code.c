/**
  ****************************************************************************
  * @file    Micro-Ex_9_2-Low-Power_modes_and_Call-of-Assembly-Code
  * @author  Buccella Pietro
  * @date    21.05.2021
  * @brief   Call-of-Assembly-Code
  *
  *
  ****************************************************************************/





#include <msp430.h>
#include <stdint.h>


enum
{
  eCYCLES_IN_ACTIVE_MODE = (8000000 / 10)   // ==> 100ms @ 8MHz
};

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	       // stop watchdog timer
    FRCTL0 = FRCTLPW | NWAITS_0;
    // Init CS
    CSCTL0_H = CSKEY_H;                // Unlock CS registers

    CSCTL1   = DCOFSEL_3 | DCORSEL;    // Set DCO to 8MHz
                                       // Set VLOCLK to ~10kHz (default)
    CSCTL2   = SELA__VLOCLK |          // Set ACLK = VLOCLK
             SELS__DCOCLK |            // Set SMCLK = DCO
             SELM__DCOCLK;             // Set MCLK = DC0
    CSCTL3   = DIVA__1 |               // Set ACLK / 1 = 10kHz
             DIVS__1 |                 // Set SMCLK / 1 = 8MHz
             DIVM__1;                  // Set MCLK / 1 = 8MHz
    CSCTL0_H = 0;                      // Lock CS registers

    // P6.6 as output to drive LED1
    P4DIR |= BIT6;                     // Set P1.0 to output direction
    P4OUT &= ~BIT6;                    // Clear P1.0 output latch for a defined power-on state
    PM5CTL0 &= ~LOCKLPM5;

    //  timerA0_init
    TA0CTL |= MC__STOP;                 // Stop the timer first
    TA0CCR0 = 5000;                     // Vmax timer = 10kHz/2Hz = 5000
    TA0CTL |= TASSEL__ACLK              // Set ACLK as source for timer
           |  ID__1                     // Do not divide
           |  TACLR                     // Clear TAxR
           |  MC__UP;                   // UP Mode selection
    TA0CCTL0 |= CCIE;      // TACCR0 interrupt enabled

    // Enable interrupts
    __bis_SR_register(GIE);
    //asm(" NOP");
    //asm(" EINT");
    //asm(" NOP");

	// Main loop
	while(1)
	{
	    __delay_cycles(eCYCLES_IN_ACTIVE_MODE);
	    // Enter LPM3 and remain in LPM3
	    // Wake up by timer every 0.5 seconds
	    __bis_SR_register(LPM3_bits);
	    __no_operation();
	    //asm(" BIS.W #0x00D0, SR");
	    //asm(" NOP");
	}
}
// Timer_A TACCR0 interrupt vector handler at 2Hz
#pragma vector= TIMER0_A0_VECTOR
__interrupt void timerA0_ISR(void)
	{
    P4OUT ^= BIT6;
	// Exit Low Power mode on reti
	_bic_SR_register_on_exit(LPM3_bits);
	//asm(" BIC.W #0x00D0, 0x0000(SP)");
	//asm(" NOP");
	}


