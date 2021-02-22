#define MAIN_C

/****************** Includes ********************/

#include "main.h"

#include <stdio.h>

#include <xparameters.h>
#include <sleep.h>
#include <xil_exception.h>

#include "Bas.h"
#include "DevInp.h"
#include "Veh.h"
#include "DevOutp.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static void deadloop( void );
static void tic( void );
static void toc( void );

/**************** Implementation ****************/

int main( void )
{
  uint8_t retVal_u8 = 0;

  // Register some exception handlers
  Xil_ExceptionInit();
  Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_DATA_ABORT_INT,     Xil_DataAbortHandler,          NULL );
  Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_PREFETCH_ABORT_INT, Xil_PrefetchAbortHandler,      NULL );
  Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_UNDEFINED_INT,      Xil_UndefinedExceptionHandler, NULL );

  // Boot up message
  setbuf(stdout, NULL); // Make STDOUT non-buffered
  putchar(0); // clear-screen
  printf( "///////// zynqFC /////////\n" );

  // Wait 200 ms power-on delay
  usleep( 200000 );

  // Initialization
  intr_disable();
  if( retVal_u8 == 0) retVal_u8 += Bas_init();
  intr_enable();
  if( retVal_u8 == 0) retVal_u8 += DevInp_init();
  if( retVal_u8 == 0) retVal_u8 += Veh_init();
  if( retVal_u8 == 0) retVal_u8 += DevOutp_init();
  if( retVal_u8 != 0)
  {
    printf( "Init ... fail\n" );
    deadloop();
  }
  printf( "Init ... ok\n" );

  // Start application task timer
  timer_start();

  // Runtime
  while( 1 )
  {
    // Wait for timer interrupt
    while( flgTimerIntr_l == 0 );
    flgTimerIntr_l = 0;

    // Execute application and measure runtime
    tic();
    app();
    toc();

    // Check if timer interrupt has triggered again
    // This indicates app executed longer then timer period -> real-time deadline missed
    if( flgTimerIntr_l )
    {
      main_s.prv_s.nrRtDeadlineMiss_u32++;
    }

    // Check button and stop at breakpoint
    if( gpio_get_BTN4() )
    {
      intr_disable();
      timer_stop();
      gpio_set_LD4( 0 );
      printf( "!!! STOP !!!\n" );
      while(1);
    }
  }

  return 0;
}

// Application cyclic task
void app( void )
{
  main_s.prv_s.nrCallApp_u32++;

  DevInp();
  Veh();
  DevOutp();
}

// Infinite loop - blinks LED
static void deadloop( void )
{
  printf( "!!! STOP !!!\n" );
  while( 1 )
  {
    gpio_set_LD4( 1 );
    usleep( 50000 );
    gpio_set_LD4( 0 );
    usleep( 50000 );
  }
}

// Start of runtime measurement
static void tic( void )
{
  XTime_GetTime( &main_s.prv_s.tiPre_ticks_u64 );
}

// Stop of runtime measurement
static void toc( void )
{
  XTime_GetTime( &main_s.prv_s.tiPost_ticks_u64 );

  // Calculate CPU ticks difference
  uint32_t diff_u32 = main_s.prv_s.tiPost_ticks_u64 - main_s.prv_s.tiPre_ticks_u64;

  // Calculate current runtime [µs]
  main_s.outp_s.tiAppRuntime_us_u32 = diff_u32 * ((double)2000000 / XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);

  // Check if this is a new max. value
  if( main_s.outp_s.tiAppRuntime_us_u32 > main_s.outp_s.tiAppRuntimeMax_us_u32 )
  {
    main_s.outp_s.tiAppRuntimeMax_us_u32 = main_s.outp_s.tiAppRuntime_us_u32;
  }

}

// EOF
