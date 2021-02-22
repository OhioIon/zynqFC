#define VEH_C

/****************** Includes ********************/

#include "main.h"
#include "dbg.h"
#include "Bas.h"
#include "DevInp.h"
#include "Veh.h"
#include "DevOutp.h"

#include <stdio.h>
#include <string.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize vehicle layer
uint8_t Veh_init( void )
{
  uint8_t retVal_u8 = 0;

  dbg( "\nInit Vehicle Layer\n" );

  // Init privates
  memset( &Veh_s.prv_s, 0, sizeof(Veh_s.prv_s) );

  // Init outputs
  memset( &Veh_s.outp_s, 0, sizeof(Veh_s.outp_s) );

  // Configure ARM instance
  if( retVal_u8 == 0 )
  {
    dbg( "- arm ...          " );
    Veh_s.arm_s.prm_s.yawThres_perml_s16 = -900;
    Veh_s.arm_s.prm_s.pitThres_perml_s16 =  900;
    Veh_s.arm_s.prm_s.rolThres_perml_s16 =  900;
    Veh_s.arm_s.prm_s.tiCyc_us_u16       =  TASK_TIME_US_D;
    Veh_s.arm_s.prm_s.tiArmDly_ms_u16    =  500;
    retVal_u8 = arm_init( &Veh_s.arm_s );
    dbgRet( retVal_u8 );
  }

  // Configure exponential instances
  if( retVal_u8 == 0 )
  {
    dbg( "- expo ...         " );
    Veh_s.expoYaw_s.prm_s.expo_perc_u8 = 10;
    retVal_u8 = expo_init( &Veh_s.expoYaw_s );

    Veh_s.expoPit_s.prm_s.expo_perc_u8 = 5;
    retVal_u8 += expo_init( &Veh_s.expoPit_s );

    Veh_s.expoRol_s.prm_s.expo_perc_u8 = 5;
    retVal_u8 += expo_init( &Veh_s.expoRol_s );
    dbgRet( retVal_u8 );
  }

  // Configure rate desired instances
  if( retVal_u8 == 0 )
  {
    dbg( "- rateDes ...      " );
    Veh_s.rateYaw_s.prm_s.rateMax_degps_u16 = 500;
    retVal_u8 = rateDes_init( &Veh_s.rateYaw_s );

    Veh_s.ratePit_s.prm_s.rateMax_degps_u16 = 500;
    retVal_u8 += rateDes_init( &Veh_s.ratePit_s );

    Veh_s.rateRol_s.prm_s.rateMax_degps_u16 = 500;
    retVal_u8 += rateDes_init( &Veh_s.rateRol_s );
    dbgRet( retVal_u8 );
  }

  // Configure PID instances
  if( retVal_u8 == 0 )
  {
    dbg( "- pid ...          " );
    Veh_s.pidYaw_s.prm_s.kp_perml_u16 = 450;
    Veh_s.pidYaw_s.prm_s.ki_perml_u16 = 0;
    Veh_s.pidYaw_s.prm_s.kd_perml_u16 = 0;
    Veh_s.pidYaw_s.prm_s.tiCyc_us_u16 = TASK_TIME_US_D;
    retVal_u8 = pid_init( &Veh_s.pidYaw_s );

    Veh_s.pidPit_s.prm_s.kp_perml_u16 = 400;
    Veh_s.pidPit_s.prm_s.ki_perml_u16 = 0;
    Veh_s.pidPit_s.prm_s.kd_perml_u16 = 0;
    Veh_s.pidPit_s.prm_s.tiCyc_us_u16 = TASK_TIME_US_D;
    retVal_u8 += pid_init( &Veh_s.pidPit_s );

    Veh_s.pidRol_s.prm_s.kp_perml_u16 = 250;
    Veh_s.pidRol_s.prm_s.ki_perml_u16 = 0;
    Veh_s.pidRol_s.prm_s.kd_perml_u16 = 0;
    Veh_s.pidRol_s.prm_s.tiCyc_us_u16 = TASK_TIME_US_D;
    retVal_u8 += pid_init( &Veh_s.pidRol_s );
    dbgRet( retVal_u8 );
  }

  return retVal_u8;
}

// Vehicle layer
void Veh( void )
{
  // Check for arm condition
  Veh_s.arm_s.inp_s.flgCon_u8     = DevInp_s.outp_s.flgCon_u8;
  Veh_s.arm_s.inp_s.yaw_perml_s16 = DevInp_s.channelYaw_s.outp_s.out_perml_s16;
  Veh_s.arm_s.inp_s.pit_perml_s16 = DevInp_s.channelPit_s.outp_s.out_perml_s16;
  Veh_s.arm_s.inp_s.rol_perml_s16 = DevInp_s.channelRol_s.outp_s.out_perml_s16;
  Veh_s.arm_s.inp_s.thr_perml_s16 = DevInp_s.channelThr_s.outp_s.out_perml_s16;
  arm( &Veh_s.arm_s );

  // DEBUG
  if( (Veh_s.prv_s.flgArmOld_u8 == 0) && (Veh_s.arm_s.outp_s.flgArmed_u8 != 0) )
  {
    printf( "!!! ARMED !!!\n" );
  }
  Veh_s.prv_s.flgArmOld_u8 = Veh_s.arm_s.outp_s.flgArmed_u8;

  // Apply expo to channels
  Veh_s.expoYaw_s.inp_s.in_perml_s16 = DevInp_s.channelYaw_s.outp_s.out_perml_s16;
  Veh_s.expoPit_s.inp_s.in_perml_s16 = DevInp_s.channelPit_s.outp_s.out_perml_s16;
  Veh_s.expoRol_s.inp_s.in_perml_s16 = DevInp_s.channelRol_s.outp_s.out_perml_s16;
  expo( &Veh_s.expoYaw_s );
  expo( &Veh_s.expoPit_s );
  expo( &Veh_s.expoRol_s );

  // Convert to desired rotation rate
  Veh_s.rateYaw_s.inp_s.in_perml_s16 = Veh_s.expoYaw_s.outp_s.out_perml_s16;
  Veh_s.ratePit_s.inp_s.in_perml_s16 = Veh_s.expoPit_s.outp_s.out_perml_s16;
  Veh_s.rateRol_s.inp_s.in_perml_s16 = Veh_s.expoRol_s.outp_s.out_perml_s16;
  rateDes( &Veh_s.rateYaw_s );
  rateDes( &Veh_s.ratePit_s );
  rateDes( &Veh_s.rateRol_s );

  // PID controller for rotation rate
  Veh_s.pidYaw_s.inp_s.sp_p1degps_s16  = Veh_s.rateYaw_s.outp_s.rate_p1degps_s16;
  Veh_s.pidPit_s.inp_s.sp_p1degps_s16  = Veh_s.ratePit_s.outp_s.rate_p1degps_s16;
  Veh_s.pidRol_s.inp_s.sp_p1degps_s16  = Veh_s.rateRol_s.outp_s.rate_p1degps_s16;
  Veh_s.pidYaw_s.inp_s.act_p1degps_s16 = DevInp_s.outp_s.yaw_p1degps_s16;
  Veh_s.pidPit_s.inp_s.act_p1degps_s16 = DevInp_s.outp_s.pit_p1degps_s16;
  Veh_s.pidRol_s.inp_s.act_p1degps_s16 = DevInp_s.outp_s.rol_p1degps_s16;
  Veh_s.pidYaw_s.inp_s.flgClrIntg_u8   = !DevInp_s.outp_s.flgStart_u8;
  Veh_s.pidPit_s.inp_s.flgClrIntg_u8   = !DevInp_s.outp_s.flgStart_u8;
  Veh_s.pidRol_s.inp_s.flgClrIntg_u8   = !DevInp_s.outp_s.flgStart_u8;
  pid( &Veh_s.pidYaw_s );
  pid( &Veh_s.pidPit_s );
  pid( &Veh_s.pidRol_s );

#ifdef DEBUG
  static uint16_t cnt_u16;
  if( Veh_s.arm_s.outp_s.flgArmed_u8 )
  {
    if( cnt_u16++ > 800 )
    {
      putchar(0);
      printf("Channel  Y %5d, P %5d, R %5d [0.1 %%]\n",
          Veh_s.expoYaw_s.outp_s.out_perml_s16,
          Veh_s.expoPit_s.outp_s.out_perml_s16,
          Veh_s.expoRol_s.outp_s.out_perml_s16);
      printf("Setpoint Y %5d, P %5d, R %5d [0.1 deg/s]\n",
          Veh_s.rateYaw_s.outp_s.rate_p1degps_s16,
          Veh_s.ratePit_s.outp_s.rate_p1degps_s16,
          Veh_s.rateRol_s.outp_s.rate_p1degps_s16);
      printf("Actual   Y %5d, P %5d, R %5d [0.1 deg/s]\n",
          DevInp_s.outp_s.yaw_p1degps_s16,
          DevInp_s.outp_s.pit_p1degps_s16,
          DevInp_s.outp_s.rol_p1degps_s16);
      printf("PID      Y %5d, P %5d, R %5d [0.1 deg/s]\n",
          Veh_s.pidYaw_s.outp_s.out_p1degps_s16,
          Veh_s.pidPit_s.outp_s.out_p1degps_s16,
          Veh_s.pidRol_s.outp_s.out_p1degps_s16);
      cnt_u16 = 0;
    }
  }
#endif

  // TODO: Move this to module
  int16_t fl_s16 = 0;
  int16_t fr_s16 = 0;
  int16_t rl_s16 = 0;
  int16_t rr_s16 = 0;
  // YAW
  fl_s16 -= Veh_s.pidYaw_s.outp_s.out_p1degps_s16 / 16;
  fr_s16 += Veh_s.pidYaw_s.outp_s.out_p1degps_s16 / 16;
  rl_s16 += Veh_s.pidYaw_s.outp_s.out_p1degps_s16 / 16;
  rr_s16 -= Veh_s.pidYaw_s.outp_s.out_p1degps_s16 / 16;
  // Pitch
  fl_s16 += Veh_s.pidPit_s.outp_s.out_p1degps_s16 / 16;
  fr_s16 += Veh_s.pidPit_s.outp_s.out_p1degps_s16 / 16;
  rl_s16 -= Veh_s.pidPit_s.outp_s.out_p1degps_s16 / 16;
  rr_s16 -= Veh_s.pidPit_s.outp_s.out_p1degps_s16 / 16;
  // Roll
  fl_s16 += Veh_s.pidRol_s.outp_s.out_p1degps_s16 / 16;
  fr_s16 -= Veh_s.pidRol_s.outp_s.out_p1degps_s16 / 16;
  rl_s16 += Veh_s.pidRol_s.outp_s.out_p1degps_s16 / 16;
  rr_s16 -= Veh_s.pidRol_s.outp_s.out_p1degps_s16 / 16;
  // Throttle
  fl_s16 += DevInp_s.channelThr_s.outp_s.out_perml_s16 * 1.5;
  fr_s16 += DevInp_s.channelThr_s.outp_s.out_perml_s16 * 1.5;
  rl_s16 += DevInp_s.channelThr_s.outp_s.out_perml_s16 * 1.5;
  rr_s16 += DevInp_s.channelThr_s.outp_s.out_perml_s16 * 1.5;
  // Min limit
  if( fl_s16 < 0 ) fl_s16 = 0;
  if( fr_s16 < 0 ) fr_s16 = 0;
  if( rl_s16 < 0 ) rl_s16 = 0;
  if( rr_s16 < 0 ) rr_s16 = 0;
  // Max limit
  if( fl_s16 > 1999 ) fl_s16 = 1999;
  if( fr_s16 > 1999 ) fr_s16 = 1999;
  if( rl_s16 > 1999 ) rl_s16 = 1999;
  if( rr_s16 > 1999 ) rr_s16 = 1999;
  // Final adjust
  if( 0 == DevInp_s.channelThr_s.outp_s.out_perml_s16 )
  {
    // Overwrite hard zero for zero throttle
    fl_s16 = 0;
    fr_s16 = 0;
    rl_s16 = 0;
    rr_s16 = 0;
  }
  else
  {
    // Shift to DSHOT minimum motor request value for all others
    fl_s16 += 48;
    fr_s16 += 48;
    rl_s16 += 48;
    rr_s16 += 48;
  }

  // Output signals to DevOutp layer
  Veh_s.outp_s.flgArmed_u8 = Veh_s.arm_s.outp_s.flgArmed_u8;
  Veh_s.outp_s.rawDataFrntLeft_u16 = fl_s16;
  Veh_s.outp_s.rawDataFrntRght_u16 = fr_s16;
  Veh_s.outp_s.rawDataRearLeft_u16 = rl_s16;
  Veh_s.outp_s.rawDataRearRght_u16 = rr_s16;

}

// EOF
