#define SPI_C

/****************** Includes ********************/

#include "spi.h"

#include <xparameters.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static volatile bool flgNrf24l01Busy_l;

/****************** Prototypes ******************/

static void spi_nrf24l01_intr(void *CallBackRef, u32 StatusEvent, u32 ByteCount);

/**************** Implementation ****************/

// Initialize SPI drivers
uint8_t spi_init( void )
{
  int retVal;

  /////////////////// nRF24L01 - PS SPI1 ///////////////////
  {
    // SPI driver init
    XSpiPs_Config *cfg_ps = XSpiPs_LookupConfig( XPAR_PS7_SPI_1_DEVICE_ID );
    if( cfg_ps == NULL ) return 1;
    retVal = XSpiPs_CfgInitialize( &nrf24l01Spi_s, cfg_ps, cfg_ps->BaseAddress );
    if( retVal != XST_SUCCESS ) return 1;

    // Reset device
    XSpiPs_Reset( &nrf24l01Spi_s );

    // Configure SPI device as master
    // Use the force slave-select option, that keeps CSB low during multi-byte transfers
    retVal = XSpiPs_SetOptions( &nrf24l01Spi_s, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION );
    if( retVal != XST_SUCCESS ) return 1;

    // Set clock pre-scaler. SCK frequency is ~ 5 MHz (166.6 MHz / 32)
    retVal = XSpiPs_SetClkPrescaler( &nrf24l01Spi_s, XSPIPS_CLK_PRESCALE_32 );
    if( retVal != XST_SUCCESS ) return 1;

    // Choose slave-select output
    XSpiPs_SetSlaveSelect( &nrf24l01Spi_s, 0 );

    // Register interrupt status handler callback
    XSpiPs_SetStatusHandler( &nrf24l01Spi_s, &nrf24l01Spi_s, spi_nrf24l01_intr );
  }

  // Done
  return 0;
}

// nRF24L01 SPI wrapper to transfer data (interrupt)
void spi_nrf24l01_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 )
{
  flgNrf24l01Busy_l = 1;
  XSpiPs_Transfer( &nrf24l01Spi_s, bufOut_au8, bufIn_au8, ByteCnt_u8 );
}

// Indicates true if transfer is still in progress (use for polled mode)
bool spi_nrf24l01_busy( void )
{
  return flgNrf24l01Busy_l;
}

// Interrupt status handler callback for nRF24L01
void spi_nrf24l01_intr(void *CallBackRef, u32 StatusEvent, u32 ByteCount)
{
  if( StatusEvent == XST_SPI_TRANSFER_DONE )
  {
    flgNrf24l01Busy_l = 0;
  }
}

// EOF
