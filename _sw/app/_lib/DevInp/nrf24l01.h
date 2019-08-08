#ifndef NRF24L01_H_
#define NRF24L01_H_

#ifdef NRF24L01_C
#define NRF24L01_SCOPE
#else
#define NRF24L01_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>
#include <stdbool.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef enum nrf24l01_dataRate_e
{
  nrf24l01_dataRate250kbps_E = 0,
  nrf24l01_dataRate1Mbps_E,
  nrf24l01_dataRate2Mbps_E,
} nrf24l01_dataRate_te;

typedef struct nrf24l01_cfg_s
{
  uint8_t  ch_u8;                  // RF channel in 2.4 GHz band (0 - 127)
  nrf24l01_dataRate_te dataRate_e; // Data rate (250 kbps, 1 Mbps, 2 Mbps)
  uint8_t  flgAutoAck_u8;          // Enable auto-acknowledge of RX packets
  uint8_t  crcSize_u8;             // CRC size in message [Byte] (1 - 2)
  uint8_t  payloadsize_u8;         // Payload size [Byte] (0 - 32)
  uint8_t  addrSize_u8;            // Address size [Byte] (3 - 5)
  uint64_t addr_u64;               // Address (only lowest 3 - 5 Bytes used)

}nrf24l01_cfg_ts;

typedef enum nrf24l01_stCfg_e
{
  nrf24l01_stCfgIdle_E = 0,    // Idle
  nrf24l01_stCfgStarted_E,     // Configuration started (disable chip)
  nrf24l01_stCfgChannel_E,     // Configure channel
  nrf24l01_stCfgRate_E,        // Configure data rate
  nrf24l01_stCfgAutoAck_E,     // Configure auto-acknowledge
  nrf24l01_stCfgCrcSize_E,     // Configure CRC size
  nrf24l01_stCfgPayloadSize_E, // Configure payload size
  nrf24l01_stCfgAddrSize_E,    // Configure address size
  nrf24l01_stCfgAddr_E,        // Configure address
  nrf24l01_stCfgFlush_E,       // Flush RX buffer
  nrf24l01_stCfgEnableChip_E,  // Enable chip (CE)
  nrf24l01_stCfgEnableWait_E,  // Wait RX settling time
} nrf24l01_stCfg_te;

typedef struct nrf24l01_prv_s
{
  nrf24l01_cfg_ts cfg_s;     // Configuration
  nrf24l01_stCfg_te stCfg_e; // Configuration state machine
  uint8_t buf_au8[33];       // Data transfer buffer

}nrf24l01_prv_ts;


typedef struct nrf24l01_s
{
  nrf24l01_prv_ts  prv_s;

}nrf24l01_ts;

/****************** Public Data *****************/

NRF24L01_SCOPE nrf24l01_ts nrf24l01_s;

/******************* Interface ******************/

NRF24L01_SCOPE uint8_t nrf24l01_init( void );

NRF24L01_SCOPE uint8_t nrf24l01_cfgStart( nrf24l01_cfg_ts * cfg_ps );
NRF24L01_SCOPE bool    nrf24l01_cfgBusy ( void );

NRF24L01_SCOPE void    nrf24l01_switchChannel( uint8_t ch_u8 );
NRF24L01_SCOPE uint8_t nrf24l01_getRxData( uint8_t *buf_au8, uint8_t bufSize_u8 );
NRF24L01_SCOPE uint8_t nrf24l01_getRxPowerDetect( void );

#endif /* NRF24L01_H_ */
