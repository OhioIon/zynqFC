#define SPI_C

/****************** Includes ********************/

#include "iic.h"

#include <xparameters.h>
#include <xiicps.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static XIicPs eeprom_s;

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize IIC drivers
uint8_t iic_init( void )
{
  int retVal;

  /////////////////// 24AA02E48 EEPROM ///////////////////
  XIicPs_Config *cfg_ps = XIicPs_LookupConfig( XPAR_XIICPS_0_DEVICE_ID );
  if( cfg_ps == NULL ) return 1;

  retVal = XIicPs_CfgInitialize( &eeprom_s, cfg_ps, cfg_ps->BaseAddress );
  if( retVal != XST_SUCCESS ) return 1;

  XIicPs_SetSClk( &eeprom_s, 100000 );

  // Done
  return 0;
}

void iic_24aa02_send( uint8_t *bufOut_pu8, uint8_t ByteCnt_u8 )
{
  XIicPs_MasterSendPolled( &eeprom_s, bufOut_pu8, ByteCnt_u8, 0x50 );
  while (XIicPs_BusIsBusy(&eeprom_s));
}

void iic_24aa02_recv( uint8_t *bufIn_pu8 , uint8_t ByteCnt_u8 )
{
  XIicPs_MasterRecvPolled( &eeprom_s, bufIn_pu8, ByteCnt_u8, 0x50 );
  while (XIicPs_BusIsBusy(&eeprom_s));
}

// EOF
