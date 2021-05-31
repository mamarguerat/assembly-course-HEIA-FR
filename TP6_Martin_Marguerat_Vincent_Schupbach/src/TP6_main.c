/**
  ****************************************************************************
  * @file    TP6_main.c
  * @author  Martin Marguerat - Vincent Schupbach
  * @date    20.05.2021
  * @brief   Lab 6 for SPI communication with TC77 on Microchip PICkit module
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

#include "HAL/HAL_MSP430.h"
#include "TC77.h"

//----------------------------------------------------------------------------
// Local functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
  initChip();

  printf("\nTP6 - SPI with TC77 temperature sensor\n");

  while(1)
  {
    // measurement itself should be executed every 1s
    // note that TC77 temperature conversion takes up to 400ms...

    const int16_t cTempBin = tc77_readTemp(); // 13-bit binary format with 0.0625 °C / bit

    if (INT_MAX == cTempBin)
    {
      printf("Wait for temp available...\n");
    }
    else
    {
//#warn 4) Implement necessary conversions for 0.1°C resolution
      const int16_t cTempConv = cTempBin*0.625; // in 0.1°C resolution

      printf("Temp = %3i.%u °C\n",cTempConv/10, abs(cTempConv%10));
    };

   __delay_cycles(8000000);
   
  }
}

