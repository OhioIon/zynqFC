#ifndef PID_H_
#define PID_H_

#ifdef PID_C
#define PID_SCOPE
#else
#define PID_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct pid_inp_s
{
  int16_t sp_degps_s16;  // Set-point rotation [°/s]
  int16_t act_degps_s16; // Actual rotation [°/s]

  uint8_t flgClrIntg_u8; // Clear integration part [bool]

}pid_inp_ts;

typedef struct pid_prm_s
{
  uint16_t kp_perml_u16; // Proportional factor [0.1 %]
  uint16_t ki_perml_u16; // Integral factor [0.1 %]
  uint16_t kd_perml_u16; // Differential factor [0.1 %]

}pid_prm_ts;

typedef struct pid_prv_s
{
  int16_t e_degps_s16; // Error signal [°/s]

  int16_t p_degps_s16; // Proportional part [°/s]
  int16_t i_degps_s16; // Integration part [°/s]
  int16_t d_degps_s16; // Differential part [°/s]

  int16_t ePrev_degps_s16; // Previous error signal [°/s]

}pid_prv_ts;

typedef struct pid_outp_s
{
  int16_t out_degps_s16; // Rotation actuation [°/s]

}pid_outp_ts;

typedef struct pid_s
{
  pid_inp_ts  inp_s;
  pid_prm_ts  prm_s;
  pid_prv_ts  prv_s;
  pid_outp_ts outp_s;

}pid_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

PID_SCOPE uint8_t pid_init( pid_ts * inst_ps );
PID_SCOPE void pid( pid_ts * inst_ps );

#endif /* PID_H_ */
