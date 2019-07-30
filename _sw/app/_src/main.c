#define MAIN_C

/****************** Includes ********************/

#include <stdint.h>
#include <stdio.h>
#include <sleep.h>

#include "Bas.h"
#include "DevInp.h"
#include "Veh.h"
#include "DevOutp.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static void deadloop( void );
static inline void app( void );

/**************** Implementation ****************/

int main( void )
{
  uint8_t retVal_u8 = 0;

  // Boot up message
  setbuf(stdout, NULL); // Make STDOUT non-buffered
  putchar(0); // clear-screen
  printf( "///////// zynqFC /////////\n" );

  // Wait 100 ms power-on delay
  usleep( 100000 );

  // Initialization
  printf( "Init ... " );
  if( retVal_u8 == 0) retVal_u8 += Bas_init();
  if( retVal_u8 == 0) retVal_u8 += DevInp_init();
  if( retVal_u8 == 0) retVal_u8 += Veh_init();
  if( retVal_u8 == 0) retVal_u8 += DevOutp_init();
  if( retVal_u8 != 0) deadloop();
  printf("ok\n");

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
      printf( "!!! STOP !!!\n" );
      gpio_set_LD4( 0 );
      while(1);
    }
  }

  return 0;
}

// Infinite loop - blinks LED
static void deadloop( void )
{
  printf("FAIL\n");
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
