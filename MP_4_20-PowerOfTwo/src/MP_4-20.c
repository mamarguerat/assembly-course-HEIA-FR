/**
  ****************************************************************************
  * @file    MP_4-20.c
  * @author  MARGUERAT Martin
  * @date    13.04.2021
  * @brief   Use of tables to compute power of 2 and log2(n)
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 13.1
  *          Optimization level: Off
  *          Level of printf_support : nofloat
  *          Heap size increased to 400bytes
  *          see http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define NBR_OF_VALUES (16) /* number of array values ... */

/// @brief  Initialzes array with power of 2
/// @param  arr - pointer to array to be initialized
/// @param  nbrOfArrayValues - number of values in the table
/// @return -
void initArray(uint16_t arr[], uint16_t nbrOfArrayValues)
{
  // TBD
}

/// @brief  Function to retrieve log base 2 of number using LUT
/// @param  arr - pointer to array with LUT
/// @param  nbrOfArrayValues - number of values in the table
/// @param  valueToComputeLog2of - value to compute the log base 2
/// @return log2(valueToComputeLog2of), 0 if not found
uint16_t getLog2(uint16_t const arr[], uint16_t nbrOfArrayValues, uint16_t valueToComputeLog2of)
{
  // TBD
  uint16_t result = 0;
  return result;
}

/// @brief  Print the values available in the array
/// @param  arr_ptr - pointer to array with values to be printed
/// @param  nbrOfArrayValues number of values in the table
/// @return -
void showArray(uint16_t *arr_ptr, uint16_t nbrOfArrayValues)
{
  printf("at address 0x%p is [", arr_ptr);
  uint16_t i=0;
  for (; i<nbrOfArrayValues-1; i++)
  {
    printf(" %u,", *(arr_ptr+i));
  };
  printf(" %u]\n", *(arr_ptr+i));
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  // a) declare array for results
  TBD...
  printf("Array to initialize : "); // show uninitialized array first
  showArray(pow2res,NBR_OF_VALUES);

  // b) initialize array with values of power of 2 (starting with 0)
  initArray(pow2res,NBR_OF_VALUES);
  printf("Initialized array : ");  // show initialized array
  showArray(pow2res,NBR_OF_VALUES);

  // c) use initialized array as LUT (Look-Up-Table) for power-of-2 conversions
  // TBD

  // d) use initialized array as LUT (Look-Up-Table) for log2(n) conversions
  for (uint16_t i=0; i<NBR_OF_VALUES; i++)
  {
    const uint16_t cValueToComputeLog2of = pow2res[i];

    // search value in LUT
    printf("The result of log2(%u) is: %u\n", cValueToComputeLog2of, getLog2(pow2res, NBR_OF_VALUES, cValueToComputeLog2of));
  };

  for (;;);
}
