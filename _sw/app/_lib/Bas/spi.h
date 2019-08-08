#ifndef SPI_H
#define SPI_H

#ifdef SPI_C
#define SPI_SCOPE
#else
#define SPI_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>
#include <stdbool.h>

#include <xspips.h>

/******************* Defines ********************/

/******************** Types *********************/

/****************** Public Data *****************/

SPI_SCOPE XSpiPs nrf24l01Spi_s;

/******************* Interface ******************/

SPI_SCOPE uint8_t spi_init( void );

SPI_SCOPE void spi_nrf24l01_transferData( uint8_t * bufOut_pu8, uint8_t *bufIn_pu8, uint8_t ByteCnt_u8 );
SPI_SCOPE bool spi_nrf24l01_busy( void );

#endif // SPI_H
