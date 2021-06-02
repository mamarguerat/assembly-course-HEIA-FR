/**
  ****************************************************************************
  * @file    MP_5-10.c
  * @author  Buccella Pietro
  * @date    21.05.2021
  * @brief   SPI initialization
  *
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdio.h>

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	    uint16_t datareceived;
	    P1SEL0 &= ~BIT6;
	    P1SEL1 |= BIT6;

        P1SEL0 &= ~BIT7;
        P1SEL1 |= BIT7;

        P2SEL0 &= ~BIT2;
        P2SEL1 |= BIT2;
        PM5CTL0 &= ~LOCKLPM5;

	    // Configure USCI_B0 for SPI operation
	    UCB0CTLW0 = UCSWRST;// Put state machine in reset
	    UCB0CTLW0 |=   UCCKPH                     // Data is captured on first UCLK edge// Inactive clock polarity is low
	              | UCMSB                         // MSB is sent first
	              | UCMODE_0                      // 3-pin SPI mode (manual STE control)
	              | UCMST                         // SPI is configured as master
	              | UCSYNC                        // eUSCI us used as SPI
	              | UCSSEL__SMCLK;                // source clock is SMCLK
	    UCB0BRW =7;
	    UCB0CTLW0 &=~UCSWRST;                     // Initialize USCI state machine
	    UCB0STATW |= UCLISTEN;

	    while(1)
	    {

	    UCB0TXBUF = 0x2A;                 // Loading of transmission buffer UCB0TXBUF with MSB of data to be tx'ed
	    while (!(UCB0IFG & UCRXIFG));     // Wait until transmission and reception complete (8 bits)
	                                            // UCTXIFG=1 indicates that transmission buffer is empty
	    UCB0IFG &= ~(UCTXIFG | UCRXIFG);  // clear Transmit and Receive Interrupt flags
	    datareceived = UCB0RXBUF;

        UCB0TXBUF = 0x20;                  // Loading of transmission buffer UCB0TXBUF with MSB of data to be tx'ed
        while (!(UCB0IFG & UCRXIFG));     // Wait until transmission and reception complete (8 bits)
                                                // UCTXIFG=1 indicates that transmission buffer is empty
        UCB0IFG &= ~(UCTXIFG | UCRXIFG);  // clear Transmit and Receive Interrupt flags
        datareceived = UCB0RXBUF;
	    }

}
