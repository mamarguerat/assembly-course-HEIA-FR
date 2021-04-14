/**
  ****************************************************************************
  * @file    MP_4_21.c
  * @author  MARGUERAT Martin
  * @date    13.04.2021
  * @brief   Use of a table to compute prime numbers.
  *          Used algorithm is described here:
  *          http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 12.2, 13.1
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


/// @brief  Initialzes array (0,1 to false and rest to true)
/// @param  arr - pointer to array to be initialized
/// @param  nbrOfArrayValues - number of values in the table
/// @return -
void initPrimes(bool arr[], uint16_t nbrOfArrayValues)
{
  for (uint16_t i=0; i<nbrOfArrayValues; i++)
     arr[i] = true;
  arr[0] = false;
  arr[1] = false;
}

/// @brief  Apply algorithm according "Sieve of Eratosthenes"
/// @note   see http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
/// @param  arr - pointer to array to be handled
/// @param  nbrOfArrayValues - number of values in the table
/// @return -
void sieveofEratosthenes(bool arr[], uint16_t nbrOfArrayValues)
{
  for (uint16_t i=0; i<nbrOfArrayValues; i++)
    if(arr[i])
      for (uint16_t j=2*i; j<nbrOfArrayValues; j+=i)
        arr[j] = false;
}

/// @brief  Get the number of primes from a boolean table
/// @param  arr - pointer to array to read from
/// @param  nbrOfArrayValues - number of values in the table
/// @return number of primes found
uint16_t getNumberOfPrimes(bool arr[], uint16_t nbrOfArrayValues)
{
  assert(nbrOfArrayValues > 1);
  uint16_t numberOfPrimes = 0; // 1 is not considered as prime
  for(unsigned int i = 0; i < nbrOfArrayValues; i++)
      if(arr[i])
          numberOfPrimes++;
  return numberOfPrimes;
}

/// @brief  Print the indizes which remain true from an array with boolean values
/// @param  arr - pointer to array with boolean values
/// @param  nbrOfArrayValues number of values in the table
/// @return -
void showPrimes(bool arr[], uint16_t nbrOfArrayValues)
{
  assert(nbrOfArrayValues > 2);
  printf("Prime numbers are [ 2");
  for (uint16_t i=3; i<nbrOfArrayValues; i++)
    if (true == (arr[i]))
      printf(", %u", i);
  printf(" ]\n");
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  // a) declare array for results (value false means non prime, true means prime)
  enum {NBR_OF_VALUES = 1000};
  bool isPrime[NBR_OF_VALUES];

  // b) initialize array (0,1 to false and rest to true)
  initPrimes(isPrime,NBR_OF_VALUES);

  // c) apply Sieve of Eratosthenes
  sieveofEratosthenes(isPrime,NBR_OF_VALUES);

  // d) show result
  const uint16_t cNbrOfPrimes = getNumberOfPrimes(isPrime,NBR_OF_VALUES);
  printf("Number of primes found between 0..%u is : %u\n", NBR_OF_VALUES, cNbrOfPrimes);
  showPrimes(isPrime, NBR_OF_VALUES);

  for (;;);
}
