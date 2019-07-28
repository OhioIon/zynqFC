#define MAIN_C

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>
#include <sleep.h>

#include <xtime_l.h>

#include "Bas.h"
#include "DevOutp.h"
#include "DevInp.h"
#include "Veh.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

XTime tic_us_u64 = 0;
XTime toc_us_u64 = 0;
XTime loop_us_u64 = 0;

/****************** Prototypes ******************/

static void deadloop( void );
static inline void app( void );

/**************** Implementation ****************/

int main( void )
{
  uint8_t retVal_u8 = 0;

  // Boot up message
  outbyte(0); // clear-screen
  xil_printf( "///////// zynqFC /////////\n" );

  // Wait 100 ms power-on delay
  usleep( 100000 );

  // Initialization
  xil_printf( "Init ... " );
  if( retVal_u8 == 0) retVal_u8 += Bas_init();
  if( retVal_u8 == 0) retVal_u8 += DevInp_init();
  if( retVal_u8 == 0) retVal_u8 += Veh_init();
  if( retVal_u8 == 0) retVal_u8 += DevOutp_init();
  if( retVal_u8 != 0) deadloop();
  xil_printf("ok\n");

  // Runtime
  while( 1 )
  {
    // Wait 10 us
    usleep( 10 );

    // Call application task
    app();

    // Check button and stop at breakpoint
    if( gpio_get_BTN4() )
    {
      xil_printf( "!!! STOP !!!\n" );
      gpio_set_LD4( 0 );
      while(1);
    }
  }

  return 0;
}

// Infinite loop - blinks LED
static void deadloop( void )
{
  xil_printf("FAIL\n");
  while( 1 )
  {
    gpio_set_LD4( 1 );
    usleep( 50000 );
    gpio_set_LD4( 0 );
    usleep( 50000 );
  }
}

// Application task
static inline void app( void )
{
  DevInp();
  Veh();
  DevOutp();
}

// EOF
