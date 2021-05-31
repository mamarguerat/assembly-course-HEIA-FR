/**
  ****************************************************************************
  * @file    HAL_SPI.c
  * @author  Bullot Dominique
  * @date    17.05.2017
  * @brief   Implementation of MSP430 HAL (Hardware Abstraction Layer) for SPI
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "HAL_SPI.h"

#include <msp430.h>


//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void enableSPI_MOSI(bool enable)
{
  if (true == enable)
  {
    P1SEL1  |= BIT6 ;    // Configure MOSI as output
  }
  else
  {
    P1SEL1 &= ~(BIT6);    // Configure MOSI as input
  }
}


void spi_init(void)
{
//#warn 1) Implement SPI initialization as commented in header
    UCB0CTLW0 |= UCSWRST;    // Put state machine in reset

    UCB0CTLW0 |= UCCKPH;     //UCB0CTLW0: Data is captured on first UCLK edge
    UCB0CTLW0 &=~UCCKPL;     //UCB0CTLW0: Inactive clock polarity is low
    UCB0CTLW0 |= UCMSB;      //UCB0CTLW0: MSB is sent first
    UCB0CTLW0 &=~(UCMODE0     //UCB0CTLW0: 3-pin SPI mode (manual STE control)
               + UCMODE1);
    UCB0CTLW0 |= UCMST;      //UCB0CTLW0: SPI is configured as master
    UCB0CTLW0 |= UCSYNC;     //UCB0CTLW0: eUSCI is used as SPI
    UCB0CTLW0 |= UCSSEL0     //UCB0CTLW0: source clock is SMCLK
               + UCSSEL1;
    UCB0BRW = 79;            //UCB0BRW: fBitClock = fBRCLK/(UCBRx + 1) = 8MHz / 80

    UCB0CTLW0 &= ~UCSWRST;   // Initialize USCI state machine
}


inline spi_write8bitData(uint8_t data)
{
  UCB0TXBUF = data;                 // Loading of transmission buffer UCB0TXBUF with MSB of data to be tx'ed
  while (!(UCB0IFG & UCRXIFG));     // Wait until transmission and reception complete (8 bits)
                                    // UCTXIFG=1 indicates that transmission buffer is empty
  UCB0IFG &= ~(UCTXIFG | UCRXIFG);  // clear Transmit and Receive Interrupt flags
}

uint16_t spi_read16bitData(void)
{
  spi_write8bitData(0x00);         // dummy write
  uint16_t data = UCB0RXBUF << 8;  // read MSBs

  spi_write8bitData(0x00);         // dummy write
  data = data | UCB0RXBUF;         // read LSBs

  return data;
}

void spi_write16bitData(uint16_t data)
{
  enableSPI_MOSI(true);                     // necessary as TC77 has bidirectional SDIO
  spi_write8bitData((uint8_t)(data >> 8));  // transmission of MSB of data
  spi_write8bitData((uint8_t)(data));       // transmission of LSB of data
  enableSPI_MOSI(false);                    // necessary as TC77 has bidirectional SDIO
}


