#define BAS_C

/****************** Includes ********************/

#include "Bas.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/
uint8_t eepData_au8[128]; // EEPROM Data (1024 Bit)
uint8_t node_au8[6];      // EUI-48 node address (MAC address)

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize base layer
uint8_t Bas_init( void )
{
  uint8_t retVal_u8 = 0;

  // Initialize HAL
  if( retVal_u8 == 0 ) retVal_u8 += gpio_init();
  if( retVal_u8 == 0 ) retVal_u8 += spi_init();
  if( retVal_u8 == 0 ) retVal_u8 += iic_init();

  // Read EEPROM data
  if( retVal_u8 == 0)
  {
    // Set EEPROM read address
    uint8_t addr_u8 = 0x00;
    iic_24aa02_send( &addr_u8, 1 );

    // Read EEPROM page (128 Byte)
    iic_24aa02_recv( eepData_au8, 128 );
  }

  // Read EUI-48 MAC address from EEPROM
  if( retVal_u8 == 0)
  {
    // Set EEPROM read address
    uint8_t addr_u8 = 0xFA;
    iic_24aa02_send( &addr_u8, 1 );

    // Read MAC (6 Byte)
    iic_24aa02_recv( node_au8, 6 );
  }

  return retVal_u8;
}

// EOF
