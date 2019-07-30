#define PID_C

/****************** Includes ********************/

#include "pid.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// PID initialization
uint8_t pid_init( pid_ts * ps )
{
  // Initialize privates
  ps->prv_s.e_degps_s16     = 0;
  ps->prv_s.ePrev_degps_s16 = 0;
  ps->prv_s.p_degps_s16     = 0;
  ps->prv_s.i_degps_s16     = 0;
  ps->prv_s.d_degps_s16     = 0;

  // Initialize outputs
  ps->outp_s.out_degps_s16 = 0;

  // Done
  return 0;
} 

// Signal processing
void pid( pid_ts * ps )
{
  // Calculate error
  ps->prv_s.e_degps_s16 = ps->inp_s.sp_degps_s16 - ps->inp_s.act_degps_s16;

  // Calculate proportional part
  ps->prv_s.p_degps_s16 = (int16_t)(ps->prv_s.e_degps_s16 * (double)ps->prm_s.kp_perml_u16 / 1000.0);

  // Calculate integration part
  if( 0U != ps->inp_s.flgClrIntg_u8 )
  {
    ps->prv_s.i_degps_s16 = 0;
  }
  else
  {
    ps->prv_s.i_degps_s16 = ps->prv_s.i_degps_s16 + (int16_t)(ps->prv_s.e_degps_s16 * (double)ps->prm_s.ki_perml_u16 / 1000.0);
  }

  // Calculate differential part
  ps->prv_s.d_degps_s16 = (int16_t)((ps->prv_s.e_degps_s16 - ps->prv_s.ePrev_degps_s16) * (double)ps->prm_s.kd_perml_u16 / 1000.0);

  // PID Output is sum of individual parts
  ps->outp_s.out_degps_s16  = ps->prv_s.p_degps_s16;
  ps->outp_s.out_degps_s16 += ps->prv_s.i_degps_s16;
  ps->outp_s.out_degps_s16 += ps->prv_s.d_degps_s16;

  // Store previous error signal
  ps->prv_s.ePrev_degps_s16 = ps->prv_s.e_degps_s16;
}

// EOF
