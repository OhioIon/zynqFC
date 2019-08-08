#define TIMER_C

/****************** Includes ********************/

#include "timer.h"

#include "main.h"

#include <xparameters.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static uint32_t cntTimerIntr_u32 = 0;

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize timer driver
uint8_t timer_init( void )
{
  int retVal;

  /////////////////// SCUTIMER_0 - CPU private timer of Cortex-A9 MPCore ///////////////////
  {
    // Timer driver init
    XScuTimer_Config *cfg_ps = XScuTimer_LookupConfig( XPAR_PS7_SCUTIMER_0_DEVICE_ID );
    if( cfg_ps == NULL ) return 1;
    retVal = XScuTimer_CfgInitialize( &timer_s, cfg_ps, cfg_ps->BaseAddr );
    if( retVal != XST_SUCCESS ) return 1;

    // Clear any pending interrupt
    XScuTimer_ClearInterruptStatus( &timer_s );

    // Enable interrupts
    XScuTimer_EnableInterrupt( &timer_s );

    // Enable Auto reload mode
    XScuTimer_EnableAutoReload( &timer_s );

    // Calculate timer value
    uint32_t timerVal_u32 = (uint32_t)TASK_TIME_US_D * (XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2000000);

    // Load timer value
    XScuTimer_LoadTimer( &timer_s, timerVal_u32 );
  }

  // Done
  return 0;
}

// Start timer
void timer_start( void )
{
  XScuTimer_Start( &timer_s );
}

// Stop timer
void timer_stop( void )
{
  XScuTimer_Stop( &timer_s );
}

// Interrupt handler (timer reached zero)
TIMER_SCOPE void timer_IntrHandler( void *p )
{
  cntTimerIntr_u32++;

  XScuTimer *ps = (XScuTimer *)p;
  XScuTimer_ClearInterruptStatus(ps);

  // Signal timer interrupt
  flgTimerIntr_l = 1;
}

// EOF
