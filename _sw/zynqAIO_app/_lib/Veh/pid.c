#define PID_C

/****************** Includes ********************/

#include "pid.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// PID initialization
uint8_t pid_init( pid_ts * inst_ps )
{
  // Initialize privates
  inst_ps->prv_s.e_degps_s16     = 0;
  inst_ps->prv_s.ePrev_degps_s16 = 0;
  inst_ps->prv_s.p_degps_s16     = 0;
  inst_ps->prv_s.i_degps_s16     = 0;
  inst_ps->prv_s.d_degps_s16     = 0;

  // Initialize outputs
  inst_ps->outp_s.out_degps_s16 = 0;

  // Done
  return 0;
} 

// Signal processing
void pid( pid_ts * inst_ps )
{
  // Calculate error
  inst_ps->prv_s.e_degps_s16 = inst_ps->inp_s.sp_degps_s16 - inst_ps->inp_s.act_degps_s16;

  // Calculate proportional part
  inst_ps->prv_s.p_degps_s16 = (int16_t)(inst_ps->prv_s.e_degps_s16 * (double)inst_ps->prm_s.kp_perml_u16 / 1000.0);

  // Calculate integration part
  if( 0U != inst_ps->inp_s.flgClrIntg_u8 )
  {
    inst_ps->prv_s.i_degps_s16 = 0;
  }
  else
  {
    inst_ps->prv_s.i_degps_s16 = inst_ps->prv_s.i_degps_s16 + (int16_t)(inst_ps->prv_s.e_degps_s16 * (double)inst_ps->prm_s.ki_perml_u16 / 1000.0);
  }

  // Calculate differential part
  inst_ps->prv_s.d_degps_s16 = (int16_t)((inst_ps->prv_s.e_degps_s16 - inst_ps->prv_s.ePrev_degps_s16) * (double)inst_ps->prm_s.kd_perml_u16 / 1000.0);

  // PID Output is sum of individual parts
  inst_ps->outp_s.out_degps_s16  = inst_ps->prv_s.p_degps_s16;
  inst_ps->outp_s.out_degps_s16 += inst_ps->prv_s.i_degps_s16;
  inst_ps->outp_s.out_degps_s16 += inst_ps->prv_s.d_degps_s16;

  // Store previous error signal
  inst_ps->prv_s.ePrev_degps_s16 = inst_ps->prv_s.e_degps_s16;
}

// EOF
