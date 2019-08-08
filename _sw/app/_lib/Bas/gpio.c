#define GPIO_C

/****************** Includes ********************/

#include "gpio.h"

#include <xgpiops.h>

/******************* Defines ********************/

// MIO PIN numbers
#define LD4             7
#define nRF24L01_CE    14
#define nRF24L01_IRQ   15
#define BTN4           50
#define BTN5           51

// EMIO PIN numbers
#define mpu6000_INT    54

/******************** Types *********************/

/***************** Private Data *****************/

static XGpioPs XGpioPs_s;

/****************** Prototypes ******************/

/**************** Implementation ****************/

uint8_t gpio_init( void )
{
  int retVal;

  /////////////////// PS-GPIO ///////////////////
  {
    // Initialize hardware/driver
    XGpioPs_Config *cfg_ps = XGpioPs_LookupConfig( XPAR_PS7_GPIO_0_DEVICE_ID );
    if( NULL == cfg_ps ) return 1;
    retVal = XGpioPs_CfgInitialize( &XGpioPs_s, cfg_ps, cfg_ps->BaseAddr );
    if( retVal != XST_SUCCESS ) return 1;

    // Self-test
    retVal = XGpioPs_SelfTest( &XGpioPs_s );
    if( retVal != XST_SUCCESS ) return 1;

    ///// MIO Pins /////

    // Make pin MIO7 (LED LD4 on Zybo) output
    XGpioPs_SetDirectionPin(&XGpioPs_s, LD4, 1 );
    XGpioPs_SetOutputEnablePin(&XGpioPs_s, LD4, 1);
    XGpioPs_WritePin( &XGpioPs_s, LD4, 0 );

    // Make pin MIO14 (CE pin on nRF24L01) output
    XGpioPs_SetDirectionPin( &XGpioPs_s, nRF24L01_CE, 1 );
    XGpioPs_SetOutputEnablePin(&XGpioPs_s, nRF24L01_CE, 1);
    XGpioPs_WritePin( &XGpioPs_s, nRF24L01_CE, 0 );

    // Make pin MIO15 (IRQ pin on nRF24L01) input
    XGpioPs_SetDirectionPin( &XGpioPs_s, nRF24L01_IRQ, 0 );

    // Make pin MIO50 (BTN4 on Zybo) input
    XGpioPs_SetDirectionPin( &XGpioPs_s, BTN4, 0 );

    // Make pin MIO51 (BTN5 on Zybo) input
    XGpioPs_SetDirectionPin( &XGpioPs_s, BTN5, 0 );

    ///// EMIO Pins /////

    // Make pin MIO54 (INT pin on MPU-6000) input
    XGpioPs_SetDirectionPin( &XGpioPs_s, mpu6000_INT, 0 );
  }

  // Done
  return 0;
}

///// MIO Pins /////

void gpio_set_LD4( bool on )
{
  XGpioPs_WritePin( &XGpioPs_s, LD4, on );
}

void gpio_nRF24L01_CE( bool on )
{
  XGpioPs_WritePin( &XGpioPs_s, nRF24L01_CE, on );
}

bool gpio_nRF24L01_IRQ( void )
{
  return (bool)XGpioPs_ReadPin( &XGpioPs_s, nRF24L01_IRQ );
}

bool gpio_get_BTN4( void )
{
  return (bool)XGpioPs_ReadPin( &XGpioPs_s, BTN4 );
}

bool gpio_get_BTN5( void )
{
  return (bool)XGpioPs_ReadPin( &XGpioPs_s, BTN5 );
}

///// EMIO Pins /////

bool gpio_mpu6000_INT( void )
{
  return (bool)XGpioPs_ReadPin( &XGpioPs_s, mpu6000_INT );
}

// EOF
