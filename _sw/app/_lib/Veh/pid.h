#ifndef PID_H_
#define PID_H_

#ifdef PID_C
#define PID_SCOPE
#else
#define PID_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct pid_inp_s
{
  int16_t sp_p1degps_s16;  // Set-point rotation [0.1 °/s]
  int16_t act_p1degps_s16; // Actual rotation [0.1 °/s]

  uint8_t flgClrIntg_u8;   // Clear integral part [bool]

}pid_inp_ts;

typedef struct pid_prm_s
{
  uint16_t kp_perml_u16; // Proportional factor [‰]
  uint16_t ki_perml_u16; // Integral factor [‰]
  uint16_t kd_perml_u16; // Differential factor [‰]

  uint16_t tiCyc_us_u16; // Loop/cycle time [µs]

}pid_prm_ts;

typedef struct pid_prv_s
{
  int32_t e_p1degps_s32;     // Error signal [0.1 °/s]
  int32_t ePrev_p1degps_s32; // Previous error signal [0.1 °/s]

  int32_t i_p1degps_s32;     // Integration part [0.1 °/s]
  int32_t d_p1degps_s32;     // Differential part [0.1 °/s]
  int32_t sum_p1degps_s32;   // Weighted sum of all parts [0.1 °/s]

  double dt_s_lf;            // Delta time [s]

}pid_prv_ts;

typedef struct pid_outp_s
{
  int16_t out_p1degps_s16; // Rotation actuation [0.1 °/s]

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

PID_SCOPE uint8_t pid_init( pid_ts * ps );
PID_SCOPE void pid( pid_ts * ps );

#endif /* PID_H_ */
