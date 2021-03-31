/**
  ****************************************************************************
  * @file    MP_4-1_template.c
  * @author  The Ghost...
  * @date    dd.mm.yyyy
  * @brief  Opérateurs de base (addition, soustraction, multiplication,
  *          division, module), op�rateurs d'accumulation et op�rateurs
  *          binaires en C
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  uint8_t char1= 33;
  uint8_t char2= 55;
  uint16_t int1 = 30000;

  uint8_t   ucResult;
  int8_t    scResult;
  uint16_t  uiResult;
  int16_t   siResult;

  uint16_t a = 0x1C;
  uint16_t b;

  ucResult  = char1 + char2;
  ucResult += char2;
  ucResult += 200;


}
