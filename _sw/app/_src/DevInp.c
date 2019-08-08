#define DEVINP_C

/****************** Includes ********************/

#include "main.h"
#include "Bas.h"
#include "DevInp.h"
#include "Veh.h"
#include "DevOutp.h"

#include <stdio.h>
#include <math.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize device input layer
uint8_t DevInp_init( void )
{
  uint8_t retVal_u8 = 0;

  // Init outputs
  memset( &DevInp_s.outp_s, 0, sizeof(DevInp_s.outp_s) );

  // Init MPU-6000 driver
  retVal_u8 += mpu6000_init();

  // Init nrf24l01 driver
  retVal_u8 += nrf24l01_init();

  // Init E-Sky ET6I 2.4 GHz 6-channel receiver
  et6i_s.prm_s.tiCyc_us_u16    = TASK_TIME_US_D;
  et6i_s.prm_s.timeoutRx_ms_u8 = 1000;
  if( retVal_u8 == 0 ) retVal_u8 += et6i_init();

  // Init moving average filter instances
  maf_init( &DevInp_s.mafZ_s );
  maf_init( &DevInp_s.mafX_s );
  maf_init( &DevInp_s.mafY_s );

  // Init rotation matrix instance
  DevInp_s.rotMatrix_s.prm_s.matrix_f[0][0] = 1.0;
  DevInp_s.rotMatrix_s.prm_s.matrix_f[0][1] = 0.0;
  DevInp_s.rotMatrix_s.prm_s.matrix_f[0][2] = 0.0;
  DevInp_s.rotMatrix_s.prm_s.matrix_f[1][0] = 0.0;
  DevInp_s.rotMatrix_s.prm_s.matrix_f[1][1] = +cos(-M_PI_4);
  DevInp_s.rotMatrix_s.prm_s.matrix_f[1][2] = -sin(-M_PI_4);
  DevInp_s.rotMatrix_s.prm_s.matrix_f[2][0] = 0.0;
  DevInp_s.rotMatrix_s.prm_s.matrix_f[2][1] = +sin(-M_PI_4);
  DevInp_s.rotMatrix_s.prm_s.matrix_f[2][2] = +cos(-M_PI_4);
  rotMatrix_init( &DevInp_s.rotMatrix_s );

  // Init channel instances
  DevInp_s.channelYaw_s.prm_s.lo_us_u16          = 1110;
  DevInp_s.channelYaw_s.prm_s.hi_us_u16          = 1870;
  DevInp_s.channelYaw_s.prm_s.deadBand_perml_u16 = 50;
  DevInp_s.channelYaw_s.prm_s.flgPosOnly_u8      = 0;
  DevInp_s.channelYaw_s.prm_s.flgReverse_u8      = 0;
  retVal_u8 += channel_init( &DevInp_s.channelYaw_s );

  DevInp_s.channelPit_s.prm_s.lo_us_u16          = 1200;
  DevInp_s.channelPit_s.prm_s.hi_us_u16          = 1887;
  DevInp_s.channelPit_s.prm_s.deadBand_perml_u16 = 20;
  DevInp_s.channelPit_s.prm_s.flgPosOnly_u8      = 0;
  DevInp_s.channelPit_s.prm_s.flgReverse_u8      = 1;
  retVal_u8 += channel_init( &DevInp_s.channelPit_s );

  DevInp_s.channelRol_s.prm_s.lo_us_u16          = 1167;
  DevInp_s.channelRol_s.prm_s.hi_us_u16          = 1876;
  DevInp_s.channelRol_s.prm_s.deadBand_perml_u16 = 20;
  DevInp_s.channelRol_s.prm_s.flgPosOnly_u8      = 0;
  DevInp_s.channelRol_s.prm_s.flgReverse_u8      = 1;
  retVal_u8 += channel_init( &DevInp_s.channelRol_s );

  DevInp_s.channelThr_s.prm_s.lo_us_u16          = 1093;
  DevInp_s.channelThr_s.prm_s.hi_us_u16          = 1746;
  DevInp_s.channelThr_s.prm_s.deadBand_perml_u16 = 100;
  DevInp_s.channelThr_s.prm_s.flgPosOnly_u8      = 1;
  DevInp_s.channelThr_s.prm_s.flgReverse_u8      = 0;
  retVal_u8 += channel_init( &DevInp_s.channelThr_s );

  return retVal_u8;
}

// Device input layer
void DevInp( void )
{
  // Get remote control input
  et6i();

  // Convert control input value to physical [‰]
  DevInp_s.channelYaw_s.inp_s.in_us_u16 = et6i_s.outp_s.ch4_us_u16;
  DevInp_s.channelPit_s.inp_s.in_us_u16 = et6i_s.outp_s.ch2_us_u16;
  DevInp_s.channelRol_s.inp_s.in_us_u16 = et6i_s.outp_s.ch1_us_u16;
  DevInp_s.channelThr_s.inp_s.in_us_u16 = et6i_s.outp_s.ch3_us_u16;
  channel( &DevInp_s.channelYaw_s );
  channel( &DevInp_s.channelPit_s );
  channel( &DevInp_s.channelRol_s );
  channel( &DevInp_s.channelThr_s );
  // TODO: improve channel by using mid calibration point
  // TODO: improve channel so output of 100 % on all channels is possible (remove some us from parameter)

  // Check for start condition (arm + none-zero throttle)
  if( (DevInp_s.outp_s.flgStart_u8                == 0) &&
      (Veh_s.arm_s.outp_s.flgArmed_u8             != 0) &&
      (DevInp_s.channelThr_s.outp_s.out_perml_s16 != 0) )
  {
    // PID integral part may start. MAF filter for IMU are frozen.
    DevInp_s.outp_s.flgStart_u8 = 1;
    printf( "!!! START !!!\n" );
  }

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

  // Calibrate IMU output (average of last 64 values. Freeze on start condition)
  DevInp_s.mafZ_s.inp_s.x_s16 = mpu6000_s.outp_s.rotZ_p1degps_s16;
  DevInp_s.mafX_s.inp_s.x_s16 = mpu6000_s.outp_s.rotX_p1degps_s16;
  DevInp_s.mafY_s.inp_s.x_s16 = mpu6000_s.outp_s.rotY_p1degps_s16;
  DevInp_s.mafZ_s.inp_s.flgEna_u8 = !DevInp_s.outp_s.flgStart_u8;
  DevInp_s.mafX_s.inp_s.flgEna_u8 = !DevInp_s.outp_s.flgStart_u8;
  DevInp_s.mafY_s.inp_s.flgEna_u8 = !DevInp_s.outp_s.flgStart_u8;
  maf( &DevInp_s.mafZ_s );
  maf( &DevInp_s.mafX_s );
  maf( &DevInp_s.mafY_s );

  // Apply rotation matrix to IMU measurement signals (sensor not aligned to frame)
  DevInp_s.rotMatrix_s.inp_s.yaw_s16 =  (mpu6000_s.outp_s.rotZ_p1degps_s16 - DevInp_s.mafZ_s.outp_s.y_s16);
  DevInp_s.rotMatrix_s.inp_s.pit_s16 = -(mpu6000_s.outp_s.rotX_p1degps_s16 - DevInp_s.mafX_s.outp_s.y_s16);
  DevInp_s.rotMatrix_s.inp_s.rol_s16 =  (mpu6000_s.outp_s.rotY_p1degps_s16 - DevInp_s.mafY_s.outp_s.y_s16);
  rotMatrix( &DevInp_s.rotMatrix_s );

  // Output signals to vehicle layer
  DevInp_s.outp_s.flgCon_u8  = et6i_s.outp_s.flgCon_u8;
  DevInp_s.outp_s.flgLost_u8 = et6i_s.outp_s.flgLost_u8;

  DevInp_s.outp_s.yaw_perml_s16 = DevInp_s.channelYaw_s.outp_s.out_perml_s16;
  DevInp_s.outp_s.pit_perml_s16 = DevInp_s.channelPit_s.outp_s.out_perml_s16;
  DevInp_s.outp_s.rol_perml_s16 = DevInp_s.channelRol_s.outp_s.out_perml_s16;
  DevInp_s.outp_s.thr_perml_s16 = DevInp_s.channelThr_s.outp_s.out_perml_s16;

  DevInp_s.outp_s.yaw_p1degps_s16 = DevInp_s.rotMatrix_s.outp_s.yaw_s16;
  DevInp_s.outp_s.pit_p1degps_s16 = DevInp_s.rotMatrix_s.outp_s.pit_s16;
  DevInp_s.outp_s.rol_p1degps_s16 = DevInp_s.rotMatrix_s.outp_s.rol_s16;
}

// EOF
