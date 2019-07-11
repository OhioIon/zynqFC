#ifndef SPI_H
#define SPI_H

#ifdef SPI_C
#define SPI_SCOPE
#else
#define SPI_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

/****************** Public Data *****************/

/******************* Interface ******************/

SPI_SCOPE uint8_t spi_init( void );

SPI_SCOPE void spi_nrf24l01_transferData( uint8_t * bufOut_pu8, uint8_t *bufIn_pu8, uint8_t ByteCnt_u8 );
SPI_SCOPE void spi_mpu6000_transferData ( uint8_t * bufOut_pu8, uint8_t *bufIn_pu8, uint8_t ByteCnt_u8 );

#endif // SPI_H
