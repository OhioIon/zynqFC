#ifndef GPIO_H_
#define GPIO_H_

#include <stdlib.h>
#include <stdint.h>

int gpio_init( void );

uint8_t gpio_get_BTN4( void );
uint8_t gpio_get_BTN5( void );
void gpio_set_LD4( uint8_t on_u8 );
void gpio_nRF24L01_CE( uint8_t on_u8 );

#endif /* GPIO_H_ */
