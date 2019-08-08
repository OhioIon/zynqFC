#ifndef GPIO_H_
#define GPIO_H_

#ifdef GPIO_C
#define GPIO_SCOPE
#else
#define GPIO_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>
#include <stdbool.h>

/******************* Defines ********************/

/******************** Types *********************/

/****************** Public Data *****************/

/******************* Interface ******************/

GPIO_SCOPE uint8_t gpio_init( void );

// MIO Pins
GPIO_SCOPE void gpio_set_LD4( bool on_l );
GPIO_SCOPE void gpio_nRF24L01_CE( bool on_l );
GPIO_SCOPE bool gpio_nRF24L01_IRQ( void );
GPIO_SCOPE bool gpio_get_BTN4( void );
GPIO_SCOPE bool gpio_get_BTN5( void );

// EMIO Pins
GPIO_SCOPE bool gpio_mpu6000_INT( void );

#endif /* GPIO_H_ */
