/**
  ****************************************************************************
  * @file    TP5_main.c
  * @author  Bullot Dominique
  * @date    17.04.2017
  * @brief   Lab 5 for SPI communication with TC77 on Microchip PICkit module
  *
  * @note    1) MOSI needs to be configured to GPIO input when
  *             reading TC77 on PICKit Serial SPI
  *
  *          Following ULP's deactivated: 1.1, 2.1, 3.1, 5.1, 5.3
  *
  *          For use of printf:
  *          see slau132 - MSP430 C-C++ Compiler Users Guide
  *          Heap size increased to 400bytes
  *          see http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler  *

              MSP430FR5969
         -----------------
        |              XIN|-
        |                 |  32KHz Crystal
      ->|RST              |
        |                 |
        |             P1.6|-> Data Out (UCB0SIMO) 1)
        |                 |
        |             P1.7|<- Data In (UCB0SOMI)
        |                 |
        |             P2.2|-> Serial Clock Out (UCB0CLK)
        |             P1.3|-> TC77 Slave Select (UCB0STE) - manual control...
        |             P1.4|-> MCB23S08 Slave Select  - manual control...
        |                 |

  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

//----------------------------------------------------------------------------
// /* -------------------- external Function Prototypes --------------------- */
//----------------------------------------------------------------------------

extern void initChip_asm();            /* Function Prototype for asm function */
extern int16_t tc77_readTemp_asm(void);/* Function Prototype for asm function */

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5; // Added to avoid a Compiler/diagnostic messages

  initChip_asm();

  printf("\nTP5 - SPI with TC77 temperature sensor\n");

  while(1)
  {
    // measurement itself should be executed every 1s
    // note that TC77 temperature conversion takes up to 400ms...

    const int16_t cTempBin = tc77_readTemp_asm(); // 13-bit binary format with 0.0625 °C / bit

    if (INT_MAX == cTempBin)
    {
      printf("Wait for temp available...\n");
    }
    else
    {

      const int16_t cTempConv = (cTempBin * 5 ) / 8;    // in 0.1°C resolution -> multiplication by 0.625
      printf("Temp = %3i.%u \n",cTempConv/10, abs(cTempConv%10));
      printf("Temp_Hex = %x \n",cTempBin);              // To print the TC77 temperature register value in hexadecimal
    };

   __delay_cycles(8000000);

  }
}


