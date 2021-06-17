/**
 ****************************************************************************
 * @file    TP_Test_Martin_Marguerat.c
 * @author  Martin Marguerat
 * @date    17.06.2021
 * @brief   Test des travaux pratique 2021
 ****************************************************************************/

#include <msp430.h> 

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------
#define UP      1
#define DOWN    0

#define TIMER_A_MAX_COUNT   400

//----------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions declaration
//----------------------------------------------------------------------------
void initChip();

//----------------------------------------------------------------------------
// main function
//----------------------------------------------------------------------------
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    initChip();

    __bis_SR_register(GIE);     // enable interrupts

    while (1);
}

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------
/// @brief  This method initializes the Clock System
/// @param  -
void initCS(void)
{
    // 3.2 Clock System Operation (User Guide p.96)
    //  After PUC, the CS module default configuration is:
    //   • LFXT is selected as the oscillator source for LFXTCLK. LFXTCLK is
    //     selected for ACLK (SELAx = 0) and ACLK is undivided (DIVAx = 0).

    CSCTL0_H = CSKEY_H;           // Unlock CS registers

    CSCTL4 &= ~LFXTOFF;           // Turn on LFXT

    CSCTL0_H = 0;                 // Lock CS registers
}

/// @brief  This method initializes the GPIO's - also check Timer usage
/// @param  -
void initGPIO(void)
{
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
                          // to activate previously configured port settings

    // Configure GPIO
    P1OUT &= ~0x01;      // Clear P1.0 output latch for a defined power-on state
    P1SEL0 |= 0x01;                 // Set P1.0 to use TimerA0
    P1SEL1 &= ~0x01;
    P1DIR |= 0x01;                  // Set P1.0 to output direction

    // LED 1 for debug and test
    //P4OUT |= 0x40;                  // Clear P4.6 output latch for a defined power-on state
    //P4DIR |= 0x40;                  // Set P4.6 to output direction
}

void initTimer(void)
{
    // TimerA0
    TA0CCTL0 &= ~CCIE;             // TA0CCR0 interrupt disabled

    TA0CTL = MC__STOP              // Stop timer first...
           | TACLR;                // ... and clear it

    TA0CTL &= ~0x200;              // Use ACLK as clock
    TA0CTL |= 0x100;
    TA0CTL &= ~0x20;               // Set to UP mode
    TA0CTL |= 0x10;
    TA0CCTL1 &= ~0x80;             // Set output mode to Set/Reset
    TA0CCTL1 |= 0x60;
    TA0CCR0 = TIMER_A_MAX_COUNT;   // Set maximum count to 400 (100%)
    TA0CCR1 = TIMER_A_MAX_COUNT;   // Set duty cycle to 0% (TA0CCR1 = 400 - dutyCycle(%)*400)

    // TimerB0
    TB0CCTL0 &= ~CCIE;             // TA0CCR0 interrupt disabled

    TB0CTL = MC__STOP              // Stop timer first...
    | TACLR;                       // ... and clear it

    TB0CTL &= ~0x200;              // Use ACLK as clock
    TB0CTL |= 0x100;
    TB0CTL &= ~0x20;               // Set to UP mode
    TB0CTL |= 0x10;
    TB0CCR0 = 32768;               // Set maximum count to 32768 (32768 Hz / 32768 counts = 1s)

    TB0CCTL0 |= CCIE;              // TB0CCR0 interrupt enabled
}

//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------
/// @brief  This method initializes the Chip
/// @param  -
void initChip(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT

    initGPIO();               // Configure GPIO

    initCS();                 // Startup clock system with max DCO setting ~8MHz

    initTimer();              // used for periodically wake-up from Low Power
}

//----------------------------------------------------------------------------
// Interrupts
//----------------------------------------------------------------------------

// Timer_A TACCR0 interrupt vector handler
#pragma vector= TIMER0_B0_VECTOR
__interrupt void timerB0_ISR(void)
{
    static unsigned char ramp = UP;     // UP/DOWN variable

    //P4OUT ^= BIT6;      // Toggles P4.6 (LED1) for debug and test
    if (ramp == DOWN)
    {
        TA0CCR1 += TIMER_A_MAX_COUNT / 10; // dutyCycle -= 10%
        if (TA0CCR1 >= TIMER_A_MAX_COUNT)
        {
            ramp = UP;      // If dutyCycle = 0% --> ramp UP
        }
    }
    else
    {
        TA0CCR1 -= TIMER_A_MAX_COUNT / 10; // dutyCycle += 10%
        if (TA0CCR1 == 0)
        {
            ramp = DOWN;    // If dutyCycle = 100% --> ramp DOWN
            TA0CCR1 = 0;
        }
    }
}
