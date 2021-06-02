/**
  ****************************************************************************
  * @file    MP_5-11/main.c
  * @author  Martin Marguerat
  * @date    02.06.2021
  * @brief   Initialisation d'une interface UART
  ****************************************************************************/
#include <msp430.h> 

//----------------------------------------------------------------------------
// Functions declaration
//----------------------------------------------------------------------------
void initGPIO(void);
void initCS(void);
void initUART(void);

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT

    initGPIO(); // Configure GPIO
    initCS();   // Startup clock system with max DCO setting ~8MHz
	initUART();
	
	__bis_SR_register(LPM3_bits | GIE); // Enter LPM3, interrupts enabled
	__no_operation();                   // For debugger

	while(1);
}

//----------------------------------------------------------------------------
// Interrupt routine
//----------------------------------------------------------------------------

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(UCA0IV)
    {
    case USCI_NONE:
        break;
    case USCI_UART_UCRXIFG:
        while(!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = UCA0RXBUF;
        __no_operation();
        break;
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}

//----------------------------------------------------------------------------
// Local functions
//----------------------------------------------------------------------------

void initGPIO(void)
{
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure GPIO
    PAOUT = PBOUT = PJOUT = 0;
    PADIR = PBDIR = 0;

    // Implement UART GPIO initialization
    P2SEL1 |= (BIT0|BIT1);  // Configure UCA1TXD et UCABRXD
    P2SEL0 &= ~(BIT0|BIT1); // Configure UCA1TXD et UCABRXD

    // SMCLK on P3.4
    P3DIR  |= BIT4;
    P3SEL1 |= BIT4;
}

void initCS(void)
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_0;

    CSCTL0_H = CSKEY_H;                       // Unlock CS registers

    CSCTL1   = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2   = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set ACLK=VLOCLK and SMCLK = MCLK = DCO
    CSCTL3   = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 (register cleared to 0...)

    CSCTL0_H = 0;
}

void initUART(void)
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;        // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int((52.083-52)*16) = 1
    UCA0BR0 = 52;               // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW = (0x55 << 8)     // UCBRSx = 0x55
            |   (   5 << 4)     // UCBRFx = 5
            |   UCOS16;
    UCA0CTLW0 &= ~UCSWRST;      // Initialize eUSCI
    UCA0IE |= UCRXIE;           // Enable USCI_A0
}
