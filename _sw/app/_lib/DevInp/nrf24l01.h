#ifndef NRF24L01_H_
#define NRF24L01_H_

#ifdef NRF24L01_C
#define NRF24L01_SCOPE
#else
#define NRF24L01_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef enum nrf24l01_dataRate_e
{
  nrf24l01_dataRate250kbps_E,
  nrf24l01_dataRate1Mbps_E,
  nrf24l01_dataRate2Mbps_E,
} nrf24l01_dataRate_te;

typedef struct nrf24l01_prv_s
{

}nrf24l01_prv_ts;

typedef struct nrf24l01_outp_s
{

}nrf24l01_outp_ts;

typedef struct nrf24l01_s
{
  nrf24l01_prv_ts  prv_s;
  nrf24l01_outp_ts outp_s;

}nrf24l01_ts;

/****************** Public Data *****************/

NRF24L01_SCOPE nrf24l01_ts nrf24l01_s;

/******************* Interface ******************/

NRF24L01_SCOPE uint8_t nrf24l01_init( void );

NRF24L01_SCOPE void nrf24l01_setChannel( uint8_t ch_u8 );
NRF24L01_SCOPE void nrf24l01_setCrcSize( uint8_t byte_u8 );
NRF24L01_SCOPE void nrf24l01_setAutoAck( uint8_t flg_u8 );
NRF24L01_SCOPE void nrf24l01_setDataRate( nrf24l01_dataRate_te rate_e );
NRF24L01_SCOPE void nrf24l01_setPayloadSize( uint8_t byte_u8 );
NRF24L01_SCOPE void nrf24l01_setAddr( uint8_t byte_u8, uint64_t addr_u64 );

NRF24L01_SCOPE void nrf24l01_enable( void );
NRF24L01_SCOPE void nrf24l01_disable( void );

NRF24L01_SCOPE uint8_t nrf24l01_getRxPowerDetect( void );
NRF24L01_SCOPE uint8_t nrf24l01_getRxData( uint8_t *buf_au8, uint8_t byteCnt_u8 );
NRF24L01_SCOPE void    nrf24l01_flushRxData( void );

#endif /* NRF24L01_H_ */
