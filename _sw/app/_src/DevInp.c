#define DEVINP_C

/****************** Includes ********************/

#include "DevInp.h"

#include <stdio.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize device input layer
uint8_t DevInp_init( void )
{
  uint8_t retVal_u8 = 0;

  if( retVal_u8 == 0 ) retVal_u8 += nrf24l01_init();

  if( retVal_u8 == 0 ) retVal_u8 += mpu6000_init();

  et6i_s.prm_s.tiCyc_us_u16    = 125;
  et6i_s.prm_s.timeoutRx_ms_u8 =  30; // TODO - fix loop time
  if( retVal_u8 == 0 ) retVal_u8 += et6i_init();

  return retVal_u8;
}

// Device input layer
void DevInp( void )
{
  // Get remote control input
  et6i();

  // DEBUG
  static uint8_t toOld_u8;
  if( (toOld_u8 == 0) && (et6i_s.outp_s.flgLost_u8 != 0) )
  {
    printf( "!!! Signal Lost !!!\n" );
  }
  if( (toOld_u8 == 1) && (et6i_s.outp_s.flgLost_u8 == 0) )
  {
    printf( "!!! Signal Restored !!!\n" );
  }
  toOld_u8 = et6i_s.outp_s.flgLost_u8;

  // Get IMU data
  mpu6000();

  // Output signals to vehicle layer
  DevInp_s.outp_s.flgCon_u8  = et6i_s.outp_s.flgCon_u8;
  DevInp_s.outp_s.flgLost_u8 = et6i_s.outp_s.flgLost_u8;

  DevInp_s.outp_s.yaw_us_u16 = et6i_s.outp_s.ch4_us_u16;
  DevInp_s.outp_s.pit_us_u16 = et6i_s.outp_s.ch2_us_u16;
  DevInp_s.outp_s.rol_us_u16 = et6i_s.outp_s.ch1_us_u16;
  DevInp_s.outp_s.thr_us_u16 = et6i_s.outp_s.ch3_us_u16;

  DevInp_s.outp_s.yaw_degps_s16 =  mpu6000_s.outp_s.rotZ_degps_s16;
  DevInp_s.outp_s.pit_degps_s16 = -mpu6000_s.outp_s.rotX_degps_s16;
  DevInp_s.outp_s.rol_degps_s16 =  mpu6000_s.outp_s.rotY_degps_s16;

}

// EOF
