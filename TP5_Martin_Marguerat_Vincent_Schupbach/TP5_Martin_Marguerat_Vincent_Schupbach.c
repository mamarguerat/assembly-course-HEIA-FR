/*****************************************************************************
  * @file    TP5_Martin_Marguerat_Vincent_Schupbach.c
  * @author  MARGUERAT Martin et SCHUPBACH Vincent
  * @date    06.05.2021
  * @brief   Donn�e de l'exercice dans le cahier des charges du TP5
  *             Blinking LED et PWM
  *
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdio.h>
#include <stdbool.h>

#include "HAL/HAL_MSP430.h"     // Inclusion de la librairie personnelle d'intialisation

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// local functions declaration
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// main function
//----------------------------------------------------------------------------
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  // Configure GPIO
  P1OUT &= ~0x01;                 // Clear P1.0 output latch for a defined power-on state
  P1DIR |= 0x01;                  // Set P1.0 to output direction
  P4OUT |= 0x40;                  // Clear P4.6 output latch for a defined power-on state
  P4DIR |= 0x40;                  // Set P4.6 to output direction

/*
  //----- 4.2 ----------------------------------------------------------------
  // Cette boucle for fait exactement 110'006 cycles d'horloge
  // La fr�quence du processeur est configur�e � 1MHz
  // Le temps allum� et le temps �teint sont �gaux, le duty cycle vaut 50%
  // La p�riode compl�te (allum�-�teint) vaut 220'012 cycles d'horloge
  // F_led = 1/(nbr_cycles * 1/F_processeur) = 1/(220'012 * 1/1MHz) = 4.5452Hz
  //    DUTY CYCLE  = 50%
  //    FREQUENCE   = 5.5452Hz
  for(;;) {
      volatile unsigned int i;            // volatile to prevent optimization

      P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR

      i = 10000;                          // SW Delay
      do i--;
      while(i != 0);
  }
*/
/*
  //----- 4.3 ----------------------------------------------------------------
  // Constante de luminosit�
  //    0   ->  duty cycle = 0%   -> Vert Compl�tement �teint
  //    25  ->  duty cycle = 25%
  //    50  ->  duty cycle = 50%
  //    75  ->  duty cycle = 75%
  //    100 ->  duty cycle = 100% -> Vert Compl�tement allum�
  unsigned int dutyCycle = 75;
  for(;;) {
      volatile unsigned int i;            // volatile to prevent optimization

      P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
      P4OUT ^= 0x40;                      // Toggle P4.6 using exclusive-OR

      i = dutyCycle*10;                   // SW Delay
      while(i != 0) i--;

      P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
      P4OUT ^= 0x40;                      // Toggle P4.6 using exclusive-OR

      i = (100-dutyCycle)*10;             // SW Delay
      while(i != 0) i--;

      //dutyCycle++;                      // Permet de passer � traveurs toutes
      //if(dutyCycle>100) dutyCycle=0;    // les valeurs du Duty Cycle
  }
*/
  //----- 4.4 ----------------------------------------------------------------
  // Utilisation du timer A0 en mode UP avec VLO comme horloge
  initChip();


  while (1);    //Boucle sans fin
}
//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

