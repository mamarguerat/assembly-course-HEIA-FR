/**
  ****************************************************************************
  * @file    TC77.h
  * @author  Bullot Dominique
  * @date    17.05.2017
  * @brief   Declarations of TC77 temperature sensor functions
  *
  ****************************************************************************/
#ifndef TC77_h
#define TC77_h

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <stdint.h>

//----------------------------------------------------------------------------
// Declarations
//----------------------------------------------------------------------------


/// @brief  Reading TC77 temperature values using direct access 
///         (without interrupts)
/// @param  -
/// @return 16 bits signed value (3 LSBs to be suppressed) 
///         unit = 0.0625 °C/bit
///         returns INT_MAX if invalid
int16_t tc77_readTemp(void);


#endif /* TC77_h */
