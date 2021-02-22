#define NRF24L01_C

/****************** Includes ********************/

#include "nrf24l01.h"

#include <string.h>
#include <stdbool.h>

#include <sleep.h>

/******************* Defines ********************/

#define CONFIG      0x00 // Configuration register
#define ENAA        0x01 // Enable 'auto acknowledge' function
#define EN_RXADDR   0x02 // Enable RX addresses
#define SETUP_AW    0x03 // Setup of address widths
#define SETUP_RETR  0x04 // Setup of automatic retransmission
#define RF_CH       0x05 // RF channel register
#define RF_SETUP    0x06 // RF setup register
#define STATUS      0x07 // Status register
#define RPD         0x09 // Received Power Detector register
#define RX_ADDR_P0  0x0A // Receive address data pipe 0
#define RX_PW_P0    0x11 // Number of bytes in RX payload in data pipe 0
#define FIFO_STATUS 0x17 // FIFO status register

#define WRITE      0x20 // Write command bit
#define RX_DR      0x40 // RX data ready status bit

/******************** Types *********************/



/***************** Private Data *****************/

/****************** Prototypes ******************/

// HAL
extern void spi_nrf24l01_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 ); // Transfer data via SPI
extern bool spi_nrf24l01_busy( void );                                                                 // Transfer busy
extern void gpio_nRF24L01_CE( uint8_t val ); // Set Chip-Enable GPIO state
extern bool gpio_nRF24L01_IRQ( void );       // Get IRQ GPIO state

// Register Access (polled)
static uint8_t nrf24l01_readReg ( uint8_t addr_u8 );
static void    nrf24l01_writeReg( uint8_t addr_u8, uint8_t data_u8 );

// Data
static void nrf24l01_flushRxData( void );

/**************** Implementation ****************/

// Initialize nRF24L01 chip
uint8_t nrf24l01_init( void )
{
  uint8_t reg_u8;

  // Init privates
  memset( &nrf24l01_s.prv_s, 0, sizeof(nrf24l01_s.prv_s) );

  // Clear chip-enable
  gpio_nRF24L01_CE( 0 );
  usleep( 20000 );

  // Check access to device
  reg_u8 = nrf24l01_readReg( CONFIG );
  if( reg_u8 == 0xFF )
  {
    return 1;
  }
  reg_u8 = nrf24l01_readReg( SETUP_RETR );
  if( reg_u8 != 0x03 )
  {
    return 1;
  }

  // Power up device / primary RX function / RX_DR interrupt
  reg_u8 |= 0x33;
  nrf24l01_writeReg( CONFIG, reg_u8 );

  // Wait power-up time Tpd2stby
  usleep( 20000 );

  // Flush RX FIFO to discard all old messages
  nrf24l01_flushRxData();

  // Done
  return 0;
}

// Start (re-)configuration of nRF24L01 device
// Return 0: ok, 1: parameter error
uint8_t nrf24l01_cfgStart( nrf24l01_cfg_ts * cfg_ps )
{
  // Check configuration
  if( cfg_ps == NULL ) return 1;
  if( cfg_ps->ch_u8 > 127 ) return 1;
  if( cfg_ps->dataRate_e > nrf24l01_dataRate2Mbps_E ) return 1;
  if( cfg_ps->crcSize_u8 < 1 ) return 1;
  if( cfg_ps->crcSize_u8 > 2 ) return 1;
  if( cfg_ps->payloadsize_u8 > 32 ) return 1;
  if( cfg_ps->addrSize_u8 < 3 ) return 1;
  if( cfg_ps->addrSize_u8 > 5 ) return 1;

  // Backup configuration
  nrf24l01_s.prv_s.cfg_s = *cfg_ps;

  // Init state machine. Processing is done in busy call.
  nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgStarted_E;

  return 0;
}

// Process configuration state machine. Returns zero once finished.
bool nrf24l01_cfgBusy ( void )
{
  bool retVal_l = 1;
  uint8_t reg_u8;
  uint8_t buf_au8[6];

  switch( nrf24l01_s.prv_s.stCfg_e )
  {
    default:
    case nrf24l01_stCfgIdle_E: // Not busy / idle
      retVal_l = 0;
      break;

    case nrf24l01_stCfgStarted_E: // Configuration started -> Clear CE pin
      gpio_nRF24L01_CE( 0 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgChannel_E;
      break;

    case nrf24l01_stCfgChannel_E: // Configure channel
      nrf24l01_writeReg( RF_CH, nrf24l01_s.prv_s.cfg_s.ch_u8 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgRate_E;
      break;

    case nrf24l01_stCfgRate_E: // Configure data rate
      reg_u8 = nrf24l01_readReg( RF_SETUP );
      if( nrf24l01_s.prv_s.cfg_s.dataRate_e == nrf24l01_dataRate250kbps_E )
      {
        reg_u8 |=  0x20;
        reg_u8 &= ~0x08;
      }
      if( nrf24l01_s.prv_s.cfg_s.dataRate_e == nrf24l01_dataRate1Mbps_E )
      {
        reg_u8 &= ~0x28;
      }
      if( nrf24l01_s.prv_s.cfg_s.dataRate_e == nrf24l01_dataRate2Mbps_E )
      {
        reg_u8 |=  0x08;
        reg_u8 &= ~0x20;
      }
      nrf24l01_writeReg( RF_SETUP, reg_u8 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgAutoAck_E;
      break;

    case nrf24l01_stCfgAutoAck_E: // Configure auto-acknowledge
      reg_u8 = nrf24l01_readReg( ENAA );
      if( nrf24l01_s.prv_s.cfg_s.flgAutoAck_u8 != 0 )
      {
        reg_u8 |= 0x01;
      }
      else
      {
        reg_u8 &= ~0x01;
      }
      nrf24l01_writeReg( ENAA, reg_u8 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgCrcSize_E;
      break;

    case nrf24l01_stCfgCrcSize_E: // Configure CRC size
      reg_u8 = nrf24l01_readReg( CONFIG );
      if( nrf24l01_s.prv_s.cfg_s.crcSize_u8 == 1 )
      {
        reg_u8 &= ~0x04;
      }
      else // 2 Byte
      {
        reg_u8 |= 0x04;
      }
      nrf24l01_writeReg( CONFIG, reg_u8 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgPayloadSize_E;
      break;

    case nrf24l01_stCfgPayloadSize_E: // Configure payload size
      nrf24l01_writeReg( RX_PW_P0, nrf24l01_s.prv_s.cfg_s.payloadsize_u8 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgAddrSize_E;
      break;

    case nrf24l01_stCfgAddrSize_E: // Configure address size
      nrf24l01_writeReg( EN_RXADDR, 0x01 ); // Only enable RX address 0
      nrf24l01_writeReg( SETUP_AW, nrf24l01_s.prv_s.cfg_s.addrSize_u8 - 2 ); // Configure address width
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgAddr_E;
      break;

    case nrf24l01_stCfgAddr_E: // Configure address
      // Prepare payload
      buf_au8[0] = RX_ADDR_P0 | WRITE;                             // Register Address + write command
      buf_au8[1] =  nrf24l01_s.prv_s.cfg_s.addr_u64 & 0xFF;        // LSByte
      buf_au8[2] = (nrf24l01_s.prv_s.cfg_s.addr_u64 >>  8) & 0xFF; // Byte 2
      buf_au8[3] = (nrf24l01_s.prv_s.cfg_s.addr_u64 >> 16) & 0xFF; // Byte 3
      buf_au8[4] = (nrf24l01_s.prv_s.cfg_s.addr_u64 >> 24) & 0xFF; // (Byte 4)
      buf_au8[5] = (nrf24l01_s.prv_s.cfg_s.addr_u64 >> 32) & 0xFF; // (Byte 5)
      // Transfer data (polled)
      spi_nrf24l01_transferData( buf_au8, buf_au8, nrf24l01_s.prv_s.cfg_s.addrSize_u8 + 1 );
      while( spi_nrf24l01_busy() );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgFlush_E;
      break;

    case nrf24l01_stCfgFlush_E: // Flush RX buffer
      nrf24l01_flushRxData();
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgEnableChip_E;
      break;

    case nrf24l01_stCfgEnableChip_E: // Enable chip and wait (CE pin)
      gpio_nRF24L01_CE( 1 );
      usleep( 70 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgEnableWait_E;
      break;

    case nrf24l01_stCfgEnableWait_E: // Wait remaining RX settling time
      usleep( 70 );
      nrf24l01_s.prv_s.stCfg_e = nrf24l01_stCfgIdle_E;

      // Done!
      retVal_l = 0;
      break;
  }

  return retVal_l;
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
  uint8_t reg_u8 = nrf24l01_readReg( RPD );
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
    // TODO: Use IRQ pin?
    // Read status register
    reg_u8 = nrf24l01_readReg( STATUS );

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
    // TODO: Fetch packet data automatically via interrupt and provide to caller here if available
    spi_nrf24l01_transferData( bufTmp_au8, bufTmp_au8, byteCnt_u8 + 1 );
    while( spi_nrf24l01_busy() );

    // Copy data to output buffer
    memcpy( buf_au8, &bufTmp_au8[1], byteCnt_u8 );

    // Clear RX FIFO so next value is new
    nrf24l01_flushRxData();

    // Clear RX_DR bit
    nrf24l01_writeReg( STATUS, flgRxDr_u8 );
  }

  return retVal_u8;
}

void nrf24l01_switchChannel( uint8_t ch_u8 )
{
  nrf24l01_writeReg( RF_CH, nrf24l01_s.prv_s.cfg_s.ch_u8 );
  nrf24l01_flushRxData();
}

// Flush RX FIFO (polled)
void nrf24l01_flushRxData( void )
{
  uint8_t buf_au8[1];

  buf_au8[0] = 0xE2; // Flush RX FIFO command
  spi_nrf24l01_transferData( buf_au8, buf_au8, 1 );
  while( spi_nrf24l01_busy() );
}

// Read register of nRF24L01 (polled)
static uint8_t nrf24l01_readReg( uint8_t addr_u8 )
{
  uint8_t buf_au8[2];

  // Write 5-bit register address into buffer
  buf_au8[0] = addr_u8 & 0x1F;
  buf_au8[1] = 0;

  // Transfer data (polled)
  spi_nrf24l01_transferData( buf_au8, buf_au8, 2 );
  while( spi_nrf24l01_busy() );

  // Return data
  return buf_au8[1];
}

// Write register of nRF24L01 (polled)
static void nrf24l01_writeReg( uint8_t addr_u8, uint8_t data_u8 )
{
  uint8_t buf_au8[2];

  // Write to device
  buf_au8[0]  = addr_u8 & 0x1F; // 5-bit Register Address
  buf_au8[0] |= WRITE;          // Set write register command bit
  buf_au8[1]  = data_u8;        // Value

  // Transfer data (polled)
  spi_nrf24l01_transferData( buf_au8, buf_au8, 2 );
  while( spi_nrf24l01_busy() );
}

// EOF
