#define IIC_C

/****************** Includes ********************/

#include "iic.h"

#include <xparameters.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static bool flgMpu6000Busy_l;

/****************** Prototypes ******************/

static void iic_mpu6000_intr(void *CallBackRef, u32 StatusEvent);

/**************** Implementation ****************/

// Initialize IIC drivers
uint8_t iic_init( void )
{
  int retVal;

  /////////////////// 24AA02E48 EEPROM ///////////////////
  {
    // Init IIC driver
    XIicPs_Config *cfg_ps = XIicPs_LookupConfig( XPAR_PS7_I2C_1_DEVICE_ID );
    if( cfg_ps == NULL ) return 1;
    retVal = XIicPs_CfgInitialize( &eeprom_s, cfg_ps, cfg_ps->BaseAddress );
    if( retVal != XST_SUCCESS ) return 1;

    // Reset device
    XIicPs_Reset( &eeprom_s );

    // Set IIC clock speed
    XIicPs_SetSClk( &eeprom_s, 400000 );
  }

  /////////////////// MPU-6000 ///////////////////
  {
    // Init IIC driver
    XIicPs_Config *cfg_ps = XIicPs_LookupConfig( XPAR_PS7_I2C_0_DEVICE_ID );
    if( cfg_ps == NULL ) return 1;
    retVal = XIicPs_CfgInitialize( &mpu6000Iic_s, cfg_ps, cfg_ps->BaseAddress );
    if( retVal != XST_SUCCESS ) return 1;

    // Reset device
    XIicPs_Reset( &mpu6000Iic_s );

    // Set IIC clock speed
    XIicPs_SetSClk( &mpu6000Iic_s, 400000 );

    // Register interrupt handler status callback
    XIicPs_SetStatusHandler( &mpu6000Iic_s, &mpu6000Iic_s, iic_mpu6000_intr );
  }

  // Done
  return 0;
}

// EEPROM IIC wrapper to send data (polled)
void iic_24aa02_send( uint8_t *bufOut_pu8, uint8_t ByteCnt_u8 )
{
  XIicPs_MasterSendPolled( &eeprom_s, bufOut_pu8, ByteCnt_u8, 0x50 );
}

// EEPROM IIC wrapper to receive data (polled)
void iic_24aa02_recv( uint8_t *bufIn_pu8 , uint8_t ByteCnt_u8 )
{
  XIicPs_MasterRecvPolled( &eeprom_s, bufIn_pu8, ByteCnt_u8, 0x50 );
}

// MPU-6000 IIC wrapper to send data (interrupt)
void iic_mpu6000_send( uint8_t *bufOut_pu8, uint8_t ByteCnt_u8 )
{
  flgMpu6000Busy_l = 1;
  XIicPs_MasterSend( &mpu6000Iic_s, bufOut_pu8, ByteCnt_u8, 0x68 );
}

// MPU-6000 IIC wrapper to receive data (interrupt)
void iic_mpu6000_recv( uint8_t *bufIn_pu8 , uint8_t ByteCnt_u8 )
{
  flgMpu6000Busy_l = 1;
  XIicPs_MasterRecv( &mpu6000Iic_s, bufIn_pu8, ByteCnt_u8, 0x68 );
}

// Indicates true if transfer is still in progress (use for polled mode)
bool iic_mpu6000_busy( void )
{
  return flgMpu6000Busy_l;
}

// Interrupt status handler callback for MPU-6000
static void iic_mpu6000_intr(void *CallBackRef, u32 StatusEvent)
{
  if( StatusEvent & (XIICPS_EVENT_COMPLETE_RECV | XIICPS_EVENT_COMPLETE_SEND) )
  {
    flgMpu6000Busy_l = 0;
  }
}

// EOF
