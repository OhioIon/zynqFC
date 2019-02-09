#define BAS_C

/****************** Includes ********************/

#include "Bas.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize base layer
uint8_t Bas_init( void )
{
  uint8_t retVal_u8 = 0;

  // Wait 100 ms power-on delay
  usleep( 100000 );

  // Initialize FPGA (check if programmed, clock enabled, reset released)
  // TODO FPGA initialization

  // Initialize HAL
  if( retVal_u8 == 0 ) retVal_u8 += gpio_init();
  if( retVal_u8 == 0 ) retVal_u8 += spi_init();

  return retVal_u8;
}

// EOF
