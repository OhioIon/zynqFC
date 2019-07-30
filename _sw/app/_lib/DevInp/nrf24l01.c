#define NRF24L01_C

/****************** Includes ********************/

#include "nrf24l01.h"

#include <string.h>
#include <sleep.h>

/******************* Defines ********************/

#define CONFIG      0x00 // Configuration register
#define ENAA        0x01 // Enable 'auto acknowledge' function
#define EN_RXADDR   0x02 // Enable RX addresses
#define SETUP_AW    0x03 // Setup of address widths
#define RF_CH       0x05 // RF channel register
#define RF_SETUP    0x06 // RF setup register
#define STATUS      0x07 // Status register
#define RPD         0x09 // Received Power Detector register
#define RX_ADDR_P0  0x0A // Receive address data pipe 0
#define RX_PW_P0    0x11 // Number of bytes in RX payload in data pipe 0
#define FIFO_STATUS 0x17 // FIFO status register

#define WRITE      0x20

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

extern void spi_nrf24l01_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 );
extern void gpio_nRF24L01_CE( uint8_t val );

static uint8_t nrf24l01_spiReadReg( uint8_t addr_u8 );
static void nrf24l01_spiWriteReg( uint8_t addr_u8, uint8_t data_u8 );

/**************** Implementation ****************/

// Initialize nRF24L01 chip
uint8_t nrf24l01_init( void )
{
  uint8_t reg_u8;

  // Clear chip-enable
  gpio_nRF24L01_CE( 0 );
  usleep( 10000 );

  // Check access to config register
  reg_u8 = nrf24l01_spiReadReg( CONFIG );
  if( reg_u8 == 0xFF )
  {
    return 1;
  }

  // Power up device / primary RX function
  reg_u8 |= 0x03;
  nrf24l01_spiWriteReg( CONFIG, reg_u8 );

  // Wait power-up time
  usleep( 10000 );

  // Flush RX FIFO to discard all old messages
  nrf24l01_flushRxData();

  // Done
  return 0;
} 

// Configure frequency channel in 2.4 GHz band
void nrf24l01_setChannel( uint8_t ch_u8 )
{
  if( ch_u8 <= 127 )
  {
    nrf24l01_spiWriteReg( RF_CH, ch_u8 );
  }
}

// Configure CRC field size
void nrf24l01_setCrcSize( uint8_t byte_u8 )
{
  uint8_t reg_u8 = nrf24l01_spiReadReg( CONFIG );
  if( byte_u8 == 1 )
  {
    reg_u8 &= ~0x04;
  }
  if( byte_u8 == 2 )
  {
    reg_u8 |= 0x04;
  }
  nrf24l01_spiWriteReg( CONFIG, reg_u8 );
}

// Enable/disable auto-acknowledge on data pipe 0
void nrf24l01_setAutoAck( uint8_t flg_u8 )
{
  uint8_t reg_u8 = nrf24l01_spiReadReg( ENAA );
  if( flg_u8 != 0 )
  {
    reg_u8 |= 0x01;
  }
  else
  {
    reg_u8 &= ~0x01;
  }
  nrf24l01_spiWriteReg( ENAA, reg_u8 );
}

// Set transmission data rate
void nrf24l01_setDataRate( nrf24l01_dataRate_te rate_e )
{
  uint8_t reg_u8 = nrf24l01_spiReadReg( RF_SETUP );
  switch( rate_e )
  {
  case nrf24l01_dataRate250kbps_E:
    reg_u8 |=  0x20;
    reg_u8 &= ~0x08;
    break;

  case nrf24l01_dataRate1Mbps_E:
    reg_u8 &= ~0x28;
    break;

  case nrf24l01_dataRate2Mbps_E:
    reg_u8 |=  0x08;
    reg_u8 &= ~0x20;
    break;

  default:
    break;
  }
  nrf24l01_spiWriteReg( RF_SETUP, reg_u8 );
}

// Set number of payload bytes
void nrf24l01_setPayloadSize( uint8_t byte_u8 )
{
  if( byte_u8 <= 32 )
  {
    nrf24l01_spiWriteReg( RX_PW_P0, byte_u8 );
  }
}

// Set address size and address
void nrf24l01_setAddr( uint8_t byte_u8, uint64_t addr_u64 )
{
  uint8_t buf_au8[6];

  if( (byte_u8 >= 3) && (byte_u8 <= 5) )
  {
    // Only enable RX address 0
    nrf24l01_spiWriteReg( EN_RXADDR, 0x01 );

    // Configure address width
    nrf24l01_spiWriteReg( SETUP_AW, byte_u8 - 2 );

    // Prepare payload
    buf_au8[0] = RX_ADDR_P0 | WRITE;      // Register Address + write command
    buf_au8[1] = addr_u64 & 0xFF;         // LSByte
    buf_au8[2] = (addr_u64 >>  8) & 0xFF; // Byte 2
    buf_au8[3] = (addr_u64 >> 16) & 0xFF; // Byte 3
    buf_au8[4] = (addr_u64 >> 24) & 0xFF; // (Byte 4)
    buf_au8[5] = (addr_u64 >> 32) & 0xFF; // (Byte 5)

    // Transfer data (polled)
    spi_nrf24l01_transferData( buf_au8, buf_au8, byte_u8 + 1 );
  }


}

// Enable reception of data
void nrf24l01_enable( void )
{
  gpio_nRF24L01_CE( 1 );
}

// Disable reception of data
void nrf24l01_disable( void )
{
  gpio_nRF24L01_CE( 0 );
}

// Check for RX power detection
uint8_t nrf24l01_getRxPowerDetect( void )
{
  uint8_t reg_u8 = nrf24l01_spiReadReg( RPD );
  return reg_u8 & 0x01;
}

// Read RX data from FIFO
// @return 0 OK, 1 No Data, 2 Parameter error
uint8_t nrf24l01_getRxData( uint8_t *buf_au8, uint8_t byteCnt_u8 )
{
  uint8_t retVal_u8 = 0;
  uint8_t reg_u8 = 0;
  uint8_t flgRxDr_u8 = 0;
  uint8_t bufTmp_au8[33] = {0};

  // Check parameters
  if( (NULL == buf_au8) || (byteCnt_u8 > 32) )
  {
    // Parameter error
    retVal_u8 = 2;
  }

  // Check for RX data
  if( retVal_u8 == 0 )
  {
    // Read status register
    reg_u8 = nrf24l01_spiReadReg( STATUS );

    // Check RX_DR bit (data ready)
    flgRxDr_u8 = reg_u8 & 0x40;
    if( 0 == flgRxDr_u8 )
    {
      // No data
      retVal_u8 = 1;
    }
  }

  // Read data if available
  if( retVal_u8 == 0 )
  {
    // Write RX fifo read command to buffer
    bufTmp_au8[0] = 0x61;

    // Transfer data (polled)
    spi_nrf24l01_transferData( bufTmp_au8, bufTmp_au8, byteCnt_u8 + 1 );

    // Copy data to output buffer
    memcpy( buf_au8, &bufTmp_au8[1], byteCnt_u8 );

    // Clear RX FIFO so next value is new
    nrf24l01_flushRxData();

    // Clear RX_DR bit
    nrf24l01_spiWriteReg( STATUS, flgRxDr_u8 );
  }

  return retVal_u8;
}

// Flush RX FIFO
void nrf24l01_flushRxData( void )
{
  uint8_t buf_au8[1];

  buf_au8[0] = 0xE2; // Flush RX FIFO command
  spi_nrf24l01_transferData( buf_au8, buf_au8, 1 );
}

// Read register of nRF24L01 via SPI
static uint8_t nrf24l01_spiReadReg( uint8_t addr_u8 )
{
  uint8_t buf_au8[2];

  // Write 5-bit register address into buffer
  buf_au8[0] = addr_u8 & 0x1F;

  // Transfer data (polled)
  spi_nrf24l01_transferData( buf_au8, buf_au8, 2 );

  // Return data
  return buf_au8[1];
}

// Write register of nRF24L01 via SPI
static void nrf24l01_spiWriteReg( uint8_t addr_u8, uint8_t data_u8 )
{
  uint8_t buf_au8[2];

  // Write to device
  buf_au8[0]  = addr_u8 & 0x1F; // 5-bit Register Address
  buf_au8[0] |= WRITE;          // Set write register command bit
  buf_au8[1]  = data_u8;        // Value

  // Transfer data (polled)
  spi_nrf24l01_transferData( buf_au8, buf_au8, 2 );
}


// EOF

