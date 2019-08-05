#define PID_C

/****************** Includes ********************/

#include "pid.h"

#include <string.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static int16_t pid_lim_s16( int32_t x );

/**************** Implementation ****************/

// PID initialization
uint8_t pid_init( pid_ts * ps )
{
  // Initialize privates
  memset( &ps->prv_s, 0, sizeof(ps->prv_s) );

  // Calculative delta-time
  ps->prv_s.dt_s_lf = (double)ps->prm_s.tiCyc_us_u16 / 1000000.0;

  // Initialize outputs
  memset( &ps->outp_s, 0, sizeof(ps->outp_s) );

  // Done
  return 0;
} 

// Signal processing
void pid( pid_ts * ps )
{
  // Calculate error signal (setpoint - actual)
  ps->prv_s.e_p1degps_s32 = ps->inp_s.sp_p1degps_s16 - ps->inp_s.act_p1degps_s16;

  // Calculate integral part
  ps->prv_s.i_p1degps_s32 = ps->prv_s.i_p1degps_s32 + ps->prv_s.e_p1degps_s32;

  // Clear integral part if desired
  if( 0U != ps->inp_s.flgClrIntg_u8 )
  {
    ps->prv_s.i_p1degps_s32 = 0;
  }

  // Limit integral part to +/- 2.000.000.000
  if( ps->prv_s.i_p1degps_s32 > 2000000000 )
  {
    ps->prv_s.i_p1degps_s32 = 2000000000;
  }
  if( ps->prv_s.i_p1degps_s32 < -2000000000 )
  {
    ps->prv_s.i_p1degps_s32 = -2000000000;
  }

  // Calculate derivative part
  ps->prv_s.d_p1degps_s32 = ps->prv_s.e_p1degps_s32 - ps->prv_s.ePrev_p1degps_s32;

  // Store previous error signal
  ps->prv_s.ePrev_p1degps_s32 = ps->prv_s.e_p1degps_s32;

  // PID output is sum of individual parts
  ps->prv_s.sum_p1degps_s32  = ((double)ps->prm_s.kp_perml_u16 / 1000.0) * ps->prv_s.e_p1degps_s32;                     // Kp * error
  ps->prv_s.sum_p1degps_s32 += ((double)ps->prm_s.ki_perml_u16 / 1000.0) * ps->prv_s.i_p1degps_s32 * ps->prv_s.dt_s_lf; // Ki * integral * dt
  ps->prv_s.sum_p1degps_s32 += ((double)ps->prm_s.kd_perml_u16 / 1000.0) * ps->prv_s.d_p1degps_s32 / ps->prv_s.dt_s_lf; // Kd * derivative / dt

  // Limit to s16 range
  ps->outp_s.out_p1degps_s16 = pid_lim_s16(ps->prv_s.sum_p1degps_s32);
}

// Limit s32 to s16 range
static int16_t pid_lim_s16( int32_t x_s32 )
{
  // Limit input
  if( x_s32 > INT16_MAX ) x_s32 = INT16_MAX;
  if( x_s32 < INT16_MIN ) x_s32 = INT16_MIN;

  // Cast to output
  return (int16_t)x_s32;
}

// EOF
