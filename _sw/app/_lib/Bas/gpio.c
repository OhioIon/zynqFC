// Includes
#include "gpio.h"

#include <xgpiops.h>

// Global Variables
static XGpioPs XGpioPs_s;

uint8_t gpio_init( void )
{
  int retVal;

  // Get configuration
  XGpioPs_Config *cfg_ps = XGpioPs_LookupConfig( XPAR_PS7_GPIO_0_DEVICE_ID );
  if( NULL == cfg_ps ) return 1;

  // Initialize hardware/driver
  retVal = XGpioPs_CfgInitialize( &XGpioPs_s, cfg_ps, cfg_ps->BaseAddr );
  if( retVal != XST_SUCCESS ) return 1;

  // Make pin MIO7 (LED LD4 on Zybo) output
  XGpioPs_SetDirectionPin(&XGpioPs_s, 7, 1 );
  XGpioPs_SetOutputEnablePin(&XGpioPs_s, 7, 1);
  XGpioPs_WritePin( &XGpioPs_s, 7, 0 );

  // Make pin MIO14 (CE pin on nRF24L01) output
  XGpioPs_SetDirectionPin( &XGpioPs_s, 14, 1 );
  XGpioPs_SetOutputEnablePin(&XGpioPs_s, 14, 1);
  XGpioPs_WritePin( &XGpioPs_s, 14, 0 );

  // Make pin MIO15 (IRQ pin on nRF24L01) input
  XGpioPs_SetDirectionPin( &XGpioPs_s, 15, 0 );

  // Make pin MIO50 (BTN4 on Zybo) input
  XGpioPs_SetDirectionPin( &XGpioPs_s, 50, 0 );

  // Make pin MIO51 (BTN5 on Zybo) input
  XGpioPs_SetDirectionPin( &XGpioPs_s, 51, 0 );

  // Make pin MIO54 (EMIO) (INT pin on MPU-6000) input
  XGpioPs_SetDirectionPin( &XGpioPs_s, 54, 0 );

  // Done
  return 0;
}

uint8_t gpio_get_BTN4( void )
{
  return (uint8_t)XGpioPs_ReadPin( &XGpioPs_s, 50 );
}

uint8_t gpio_get_BTN5( void )
{
  return (uint8_t)XGpioPs_ReadPin( &XGpioPs_s, 51 );
}

void gpio_set_LD4( uint8_t on )
{
	XGpioPs_WritePin( &XGpioPs_s, 7, (on != 0) );
}

void gpio_nRF24L01_CE( uint8_t on )
{
  XGpioPs_WritePin( &XGpioPs_s, 14, (on != 0) );
}

