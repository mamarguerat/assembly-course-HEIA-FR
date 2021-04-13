/**
  ****************************************************************************
  * @file    MP_4_3-MARGUERAT.c
  * @author  Martin MARGUERAT
  * @date    13.04.2021
  * @brief  Le but de cet exercice est de réaliser le même programme que pour
  *         l'exercice MP3.3, mais cette fois ci en C (avec CCS) au lieu en
  *         assembleur: Il s'agit de calculer la moyenne des valeurs signées
  *         sur 8bit contenues dans un tableau.
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>

enum {eNBR_VALUES = 16};
signed char values[eNBR_VALUES] = {
 122, 23, 31, 27,
 54, -2, -17, -5,
 112, 13, 56, 65,
 21, 87, -32, -54
};

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  PM5CTL0 &= ~LOCKLPM5;

  signed int sum = 0;
  float mean = 0;
  for(unsigned char i = 0; i < eNBR_VALUES; i++) {
     sum += values[i];
  }

  mean = (float)sum / eNBR_VALUES;

  while(1); // wait forever...git
}
