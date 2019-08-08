#define INTR_C

/****************** Includes ********************/

#include "intr.h"

#include <xparameters.h>
#include <xscugic.h>

#include "timer.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

static XScuGic XScuGic_s; // Interrupt controller instance

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize interrupt controller
uint8_t intr_init( void )
{
  int16_t retVal_s16;

  /////////////////// CPU GIC ///////////////////
  {
    // Get configuration
    XScuGic_Config *intrCfg_ps = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );
    if( NULL == intrCfg_ps ) return 1;

    // Initialize hardware/driver
    retVal_s16 = XScuGic_CfgInitialize( &XScuGic_s, intrCfg_ps, intrCfg_ps->CpuBaseAddress );
    if( XST_SUCCESS != retVal_s16 ) return 1;

    // Self-test
    retVal_s16 = XScuGic_SelfTest( &XScuGic_s );
    if( XST_SUCCESS != retVal_s16 ) return 1;

    // Register interrupt controller interrupt handler to hardware IRQ
    Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &XScuGic_s );

    // Register/enable XScuTimer interrupt handler (main task timer)
    retVal_s16 = XScuGic_Connect( &XScuGic_s, XPAR_SCUTIMER_INTR, (Xil_ExceptionHandler)timer_IntrHandler, &timer_s );
    if( XST_SUCCESS != retVal_s16 ) return 1;
    XScuGic_Enable( &XScuGic_s, XPAR_SCUTIMER_INTR );
  }

  // Done
  return 0;
}

// Global interrupt enable
void intr_enable( void )
{
  Xil_ExceptionEnable();
}

// Global interrupt disable
void intr_disable( void )
{
  Xil_ExceptionDisable();
}

// EOF
