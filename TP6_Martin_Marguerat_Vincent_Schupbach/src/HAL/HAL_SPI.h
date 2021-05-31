/**
  ****************************************************************************
  * @file    HAL_SPI.h
  * @author  Scherwey Roland
  * @date    09.04.2015
  * @brief   Declaration of MSP430 HAL (Hardware Abstraction Layer) for SPI
  *
  ****************************************************************************/
#ifndef HAL_SPI_h
#define HAL_SPI_h

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// Declarations
//----------------------------------------------------------------------------

/// @brief  This method allows to read 16 bit of data on the SPI interface
///         using direct access to the registers
/// @note   This is done using direct access to the registers with blocking
///         until Tx and Rx process is finished
/// @return 16 bit value read
uint16_t spi_read16bitData(void);

/// @brief  This method allows to write 16 bit of data on the SPI interface
/// @note   This is done using direct access to the registers with blocking
///         until Tx and Rx process is finished
/// @param  data - 16 bit value to be written
void spi_write16bitData(uint16_t data);


/// @brief  This method allows to control the MOSI line (necessary for TC77)
/// @param  enable true  -> enables MOSI
///                false -> disables MOSI (configures as I/O as input)
void enableSPI_MOSI(bool enable);

/// @brief  This method initializes the SPI to use MSP430FR5969 Launchpad
///         together with PICKit Demo board
///         e_USCI is initialized to operate as:
///         - SPI 8-bit Master @100 KHz
///         - Inactive Clock polarity is high
///         - 3-pin SPI mode (chip select is handled manually)
///         - source clock is SMCLK
void spi_init(void);

#endif /* HAL_SPI_h */

