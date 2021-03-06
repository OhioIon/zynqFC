#define DEVOUTP_C

/****************** Includes ********************/

#include "main.h"
#include "dbg.h"
#include "Bas.h"
#include "DevInp.h"
#include "Veh.h"
#include "DevOutp.h"

#include <stdio.h>
#include <xparameters.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize device output layer
uint8_t DevOutp_init( void )
{
  uint8_t retVal_u8 = 0;

  dbg( "\nInit Device Output Layer\n" );

  // Init DSHOT waveform generator
  if( retVal_u8 == 0 )
  {
    dbg( "- dshotWave ...    " );
    DevOutp_s.dshotWaveform_s.prm_s.addrBas_pv = (void*)XPAR_DSHOTWAVEFORM_0_0;
    DevOutp_s.dshotWaveform_s.prm_s.speed_kbps_u16 = 300; // DShot300
    retVal_u8 = dshotWaveform_init( &DevOutp_s.dshotWaveform_s );
    dbgRet( retVal_u8 );
  }

  // Init motor DSHOT protocol drivers
  if( retVal_u8 == 0 )
  {
    dbg( "- dshot ...        " );
    DevOutp_s.dshotMotFrntLeft_s.prm_s.addrBas_pv = (void*)XPAR_DSHOT_0_0;
    retVal_u8 = dshot_init( &DevOutp_s.dshotMotFrntLeft_s );

    DevOutp_s.dshotMotFrntRght_s.prm_s.addrBas_pv = (void*)XPAR_DSHOT_1_0;
    retVal_u8 += dshot_init( &DevOutp_s.dshotMotFrntRght_s );

    DevOutp_s.dshotMotRearLeft_s.prm_s.addrBas_pv = (void*)XPAR_DSHOT_2_0;
    retVal_u8 += dshot_init( &DevOutp_s.dshotMotRearLeft_s );

    DevOutp_s.dshotMotRearRght_s.prm_s.addrBas_pv = (void*)XPAR_DSHOT_3_0;
    retVal_u8 += dshot_init( &DevOutp_s.dshotMotRearRght_s );
    dbgRet( retVal_u8 );
  }

  // Init LED
  if( retVal_u8 == 0 )
  {
    dbg( "- led ...          " );
    DevOutp_s.led4_s.prm_s.tiCyc_us_u16       = TASK_TIME_US_D;
    DevOutp_s.led4_s.prm_s.tiBlinkSlow_ms_u16 = 1000;
    DevOutp_s.led4_s.prm_s.tiBlinkFast_ms_u16 = 200;
    retVal_u8 = led_init( &DevOutp_s.led4_s );
    dbgRet( retVal_u8 );
  }

  return retVal_u8;
}

// Device output layer
void DevOutp( void )
{
  // Only output to motor in case armed
  if( (Veh_s.outp_s.flgArmed_u8 != 0) && (DevInp_s.outp_s.flgLost_u8 == 0) )
  {
    // Output desired throttle values via DSHOT to motor ESCs
    DevOutp_s.dshotMotFrntLeft_s.inp_s.thrData_u16 = Veh_s.outp_s.rawDataFrntLeft_u16;
    DevOutp_s.dshotMotFrntRght_s.inp_s.thrData_u16 = Veh_s.outp_s.rawDataFrntRght_u16;
    DevOutp_s.dshotMotRearLeft_s.inp_s.thrData_u16 = Veh_s.outp_s.rawDataRearLeft_u16;
    DevOutp_s.dshotMotRearRght_s.inp_s.thrData_u16 = Veh_s.outp_s.rawDataRearRght_u16;
    dshot( &DevOutp_s.dshotMotFrntLeft_s );
    dshot( &DevOutp_s.dshotMotFrntRght_s );
    dshot( &DevOutp_s.dshotMotRearLeft_s );
    dshot( &DevOutp_s.dshotMotRearRght_s );

#ifdef DEBUG
    static uint16_t cnt_u16 = 0;
    if( cnt_u16++ > 800 )
    {
      printf("Motors Raw:\n");
      printf("%4d, %4d\n",
          DevOutp_s.dshotMotFrntLeft_s.inp_s.thrData_u16,
          DevOutp_s.dshotMotFrntRght_s.inp_s.thrData_u16);
      printf("%4d, %4d\n",
          DevOutp_s.dshotMotRearLeft_s.inp_s.thrData_u16,
          DevOutp_s.dshotMotRearRght_s.inp_s.thrData_u16);
      cnt_u16 = 0;
    }
#endif

  }


  // Use LD4 on Zybo board to indicate RX connection state
  DevOutp_s.led4_s.inp_s.fct_e = led_fctBlinkSlow_E;

  if( et6i_s.outp_s.flgBind_u8 )
  {
    DevOutp_s.led4_s.inp_s.fct_e = led_fctBlinkFast_E;
  }

  if( et6i_s.outp_s.flgCon_u8 )
  {
    DevOutp_s.led4_s.inp_s.fct_e = led_fctOn_E;
  }

  if( et6i_s.outp_s.flgLost_u8 )
  {
    DevOutp_s.led4_s.inp_s.fct_e = led_fctOff_E;
  }

  led( &DevOutp_s.led4_s );
  gpio_set_LD4( DevOutp_s.led4_s.outp_s.flgOn_u8 );

}


// EOF
