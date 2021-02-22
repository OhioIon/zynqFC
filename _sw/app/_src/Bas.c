#define BAS_C

/****************** Includes ********************/

#include "Bas.h"
#include "dbg.h"

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

  dbg( "\nInit Base Layer\n" );

  if( retVal_u8 == 0 )
  {
    dbg( "- gpio ...         " );
    retVal_u8 = gpio_init();
    dbgRet( retVal_u8 );
  }

  if( retVal_u8 == 0 )
  {
    dbg( "- spi ...          " );
    retVal_u8 = spi_init();
    dbgRet( retVal_u8 );
  }
  if( retVal_u8 == 0 )
  {
    dbg( "- iic ...          " );
    retVal_u8 = iic_init();
    dbgRet( retVal_u8 );
  }
  if( retVal_u8 == 0 )
  {
    dbg( "- timer ...        " );
    retVal_u8 = timer_init();
    dbgRet( retVal_u8 );
  }
  if( retVal_u8 == 0 )
  {
    dbg( "- intr ...         " );
    retVal_u8 = intr_init();
    dbgRet( retVal_u8 );
  }

  // Read EEPROM data
  // TODO: Move to DevInp layer
  if( retVal_u8 == 0)
  {
    dbg( "- eep ...          " );

    // Set EEPROM read address
    uint8_t addr_u8 = 0x00;
    iic_24aa02_send( &addr_u8, 1 );

    // Read EEPROM user-data (128 Byte)
    iic_24aa02_recv( eepData_au8, 128 );

    // Set EEPROM read address
    addr_u8 = 0xFA;
    iic_24aa02_send( &addr_u8, 1 );

    // Read EUI-48 node-address (e.g. MAC-address) from EEPROM (6 Byte)
    iic_24aa02_recv( node_au8, 6 );

    dbgRet( 0 );
  }

  return retVal_u8;
}

// EOF
