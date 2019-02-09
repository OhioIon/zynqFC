#define SPI_C

/****************** Includes ********************/

#include "spi.h"

#include <xparameters.h>
#include <xiic.h> // TODO remove
#include <xspips.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static XSpiPs nrf24l01Spi_s;
static XIic   mpu6000Iic_s;

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize SPI drivers
uint8_t spi_init( void )
{
  int retVal;

  /////////////////// nRF24L01 ///////////////////

  // SPI driver init
  XSpiPs_Config *cfg_ps = XSpiPs_LookupConfig( XPAR_PS7_SPI_1_DEVICE_ID );
  retVal = XSpiPs_CfgInitialize( &nrf24l01Spi_s, cfg_ps, cfg_ps->BaseAddress );
  if( retVal != XST_SUCCESS ) return 1;

  // Configure SPI device as master
  // Use the force slave-select option, that keeps CSB low during multi-byte transfers
  retVal = XSpiPs_SetOptions( &nrf24l01Spi_s, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION );
  if( retVal != XST_SUCCESS ) return 1;

  // Set clock pre-scaler. SCK frequency is ~ 5 MHz (166.6 MHz / 32)
  retVal = XSpiPs_SetClkPrescaler( &nrf24l01Spi_s, XSPIPS_CLK_PRESCALE_32 );
  if( retVal != XST_SUCCESS ) return 1;

  // Choose slave-select output
  XSpiPs_SetSlaveSelect( &nrf24l01Spi_s, 0 );

  /////////////////// MPU-6000 ///////////////////

  // TODO: Replace I2C interface with SPI interface
  retVal = XIic_Initialize( &mpu6000Iic_s, XPAR_AXI_IIC_1_DEVICE_ID );
  if( retVal != XST_SUCCESS ) return 1;
  retVal = XIic_SetAddress( &mpu6000Iic_s, XII_ADDR_TO_SEND_TYPE, 0x68 );
  if( retVal != XST_SUCCESS ) return 1;

  // Done
  return 0;
}

// nRF24L01 SPI wrapper
void spi_nrf24l01_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 )
{
  XSpiPs_PolledTransfer( &nrf24l01Spi_s, bufOut_au8, bufIn_au8, ByteCnt_u8 );
}

// MPU-6000 SPI wrapper
void spi_mpu6000_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 )
{
  // TODO: replace I2C interface with SPI interface
  if( bufOut_au8[0] & 0x80 )
  {
    // Read IIC
    uint8_t buf_u8[1];

    // Write register address
    buf_u8[0] = (bufOut_au8[0] & 0x7F);
    XIic_Send( mpu6000Iic_s.BaseAddress, mpu6000Iic_s.AddrOfSlave, buf_u8, 1, XIIC_STOP );

    // Read specified number of register
    XIic_Recv( mpu6000Iic_s.BaseAddress, mpu6000Iic_s.AddrOfSlave, bufIn_au8, ByteCnt_u8 - 1, XIIC_STOP );

    // Copy values to correct position in SPI transfer buffer (skip address byte)
    uint8_t idx_u8;
    for(idx_u8 = ByteCnt_u8 - 1; idx_u8 != 0; idx_u8-- )
    {
      bufIn_au8[idx_u8] = bufIn_au8[idx_u8-1];
    }
  }
  else
  {
    // Write IIC
    XIic_Send( mpu6000Iic_s.BaseAddress, mpu6000Iic_s.AddrOfSlave, bufOut_au8, ByteCnt_u8, XIIC_STOP );
  }
}

// EOF
