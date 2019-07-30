#ifndef ARM_H_
#define ARM_H_

#ifdef ARM_C
#define ARM_SCOPE
#else
#define ARM_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct arm_inp_s
{
  uint8_t flgCon_u8;     // Remote control connected [bool]

  int16_t yaw_perml_s16; // Yaw channel value [0.1 %]
  int16_t pit_perml_s16; // Pitch channel value [0.1 %]
  int16_t rol_perml_s16; // Roll channel value [0.1 %]
  int16_t thr_perml_s16; // Throttle channel value [0.1 %]

}arm_inp_ts;

typedef struct arm_prm_s
{
  int16_t yawThres_perml_s16; // Yaw threshold value [0.1 %]
  int16_t pitThres_perml_s16; // Pitch threshold value [0.1 %]
  int16_t rolThres_perml_s16; // Roll threshold value [0.1 %]

  uint16_t tiCyc_us_u16;      // Loop/cycle time [us]
  uint16_t tiArmDly_ms_u16;   // Arm delay [ms]

}arm_prm_ts;

typedef struct arm_prv_s
{
  uint8_t flgYaw_u8; // Yaw OK
  uint8_t flgPit_u8; // Pitch OK
  uint8_t flgRol_u8; // Roll OK
  uint8_t flgThr_u8; // Throttle OK

  uint8_t  flgArmed_u8;     // Armed signal
  uint32_t cntArmed_us_u32; // Armed debounce timer [us]

}arm_prv_ts;

typedef struct arm_outp_s
{
  uint8_t flgArmed_u8; // Armed status (debounced and latched)

}arm_outp_ts;

typedef struct arm_s
{
  arm_inp_ts  inp_s;
  arm_prm_ts  prm_s;
  arm_prv_ts  prv_s;
  arm_outp_ts outp_s;

}arm_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

ARM_SCOPE uint8_t arm_init( arm_ts * ps );
ARM_SCOPE void arm( arm_ts * ps );

#endif /* ARM_H_ */
