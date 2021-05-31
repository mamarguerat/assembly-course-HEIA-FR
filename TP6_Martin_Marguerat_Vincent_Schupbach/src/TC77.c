/**
  ****************************************************************************
  * @file    TC77.c
  * @author  Bullot Dominique
  * @date    17.05.2017
  * @brief   Implementation of TC77 temperature sensor functions
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "TC77.h"
#include "HAL/HAL_SPI.h"

#include <msp430.h>
#include <limits.h>

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

void setTC77_CS(void)
{
  P1OUT &= ~BIT3;               // Enable of CS on P1.3
}

void resetTC77_CS(void)
{
  P1OUT |= BIT3;                // Disable of CS on P1.3
}

//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

int16_t tc77_readTemp(void)
{
  int16_t data = 0;
  //#warn 3) Implement read of TC77 temperature sensor
  setTC77_CS();                 // enable TC77 CS pin
  data = spi_read16bitData();   // read data with spi_read16bitData()
  resetTC77_CS();               // disable TC77 CS pin
  if(!(data | BIT2))               // check if first conversion is available after power up
  {
      // if yes replace data with INT_MAX
      data = INT_MAX;
  }
  else
  {
      // else return data without 3 LSB's
      data = data >> 3;
  }
  return data;
}

