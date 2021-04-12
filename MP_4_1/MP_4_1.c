/**
  ****************************************************************************
  * @file    MP_4-1_template.c
  * @author  Martin MARGUERAT
  * @date    12.04.2021
  * @brief  Opérateurs de base (addition, soustraction, multiplication,
  *          division, module), opérateurs d'accumulation et opérateurs
  *          binaires en C
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>

void executeAdditions_a();
void executeMultiplications_b();
void executeBinaryOperators_c();
void executeShiftOperators_d();
void executeShiftOperators_e();

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  executeAdditions_a();
  executeMultiplications_b();
  executeBinaryOperators_c();
  executeShiftOperators_d();
  executeShiftOperators_e();

  while(1); //Boucle infinie -> garde-fou
}

void executeAdditions_a()
{
    uint8_t aResult;
    uint8_t char1= 33;
    uint8_t char2= 55;
    uint16_t int1 = 30000;

    aResult  = char1 + int1;    //aResult = 30033
    aResult += char2;           //aResult = 30088
    aResult += 200;             //aResult = 30288
}

void executeMultiplications_b()
{
    uint8_t char1= 33;
    uint8_t char2= 55;
    uint16_t int1 = 600;
    uint16_t bResult;

    bResult  = char1 * int1;    //bResult = 19800
    bResult *= char2;           //bResult = -25112 (dépassement)
    bResult *= 20;              //bResult = 22048 (dépassement)
}

void executeBinaryOperators_c()
{
    int a = 10;
    int b = 20;
    int cResult;

    cResult = a | b;    //cResult = 30
    cResult = a & b;    //cResult = 0
    cResult = a ^ b;    //cResult = 30
    cResult = ~a;       //cResult = -11
}

void executeShiftOperators_d()
{
    uint16_t a = 0x1C;
    uint16_t dResult;

    dResult = a << 2;   //dResult = 0x70
    dResult = a >> 4;   //dResult = 0x01
    a <<= 3;            //a = 0xE0
}

void executeShiftOperators_e()
{
    uint16_t a = 0x2A;
    uint16_t eResult;

    eResult = a << 1;   //eResult = 0x54
    eResult = a >> 3;   //eResult = 0x05
    a <<= eResult;      //a = 0x540
}
