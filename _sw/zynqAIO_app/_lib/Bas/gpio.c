// Includes
#include <xparameters.h>
#include <xgpiops.h>

// Global Variables
static XGpioPs inst_s;

uint8_t gpio_init( void )
{
	int retVal;

	// Load PS-GPIO core config and init hardware
	XGpioPs_Config *cfg_ps = XGpioPs_LookupConfig( XPAR_PS7_GPIO_0_DEVICE_ID );
	retVal = XGpioPs_CfgInitialize( &inst_s, cfg_ps, cfg_ps->BaseAddr );
	if( retVal != XST_SUCCESS ) return 1;

	// Make pin MIO7 (LED LD4 on Zybo) output
	XGpioPs_SetDirectionPin(&inst_s, 7, 1 );
	XGpioPs_SetOutputEnablePin(&inst_s, 7, 1);

  // Make pin MIO14 (CE pin on nRF24L01 transceiver) output
  XGpioPs_SetDirectionPin( &inst_s, 14, 1 );
  XGpioPs_SetOutputEnablePin(&inst_s, 14, 1);

  // Make pin MIO50 (BTN4 on Zybo) input
  XGpioPs_SetDirectionPin( &inst_s, 50, 0 );

	// Done
	return 0;
}

u8 gpio_get_BTN4( void )
{
  return (u8)XGpioPs_ReadPin( &inst_s, 50 );
}

void gpio_set_LD4( uint8_t on )
{
	XGpioPs_WritePin( &inst_s, 7, (on != 0) );
}

void gpio_nRF24L01_CE( uint8_t on )
{
  XGpioPs_WritePin( &inst_s, 14, (on != 0) );
}

