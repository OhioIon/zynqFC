#ifndef CHANNEL_H_
#define CHANNEL_H_

#ifdef CHANNEL_C
#define CHANNEL_SCOPE
#else
#define CHANNEL_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct channel_inp_s
{
  uint16_t in_us_u16; // Channel PWM width [µs]

}channel_inp_ts;

typedef struct channel_prm_s
{
  uint8_t  flgPosOnly_u8;      // Positive output only
  uint8_t  flgReverse_u8;      // Reverse direction
  uint16_t lo_us_u16;          // min. input value [µs]
  uint16_t hi_us_u16;          // max. input value [µs]
  uint16_t deadBand_perml_u16; // Deadband around zero value [0.1 %]

}channel_prm_ts;

typedef struct channel_prv_s
{

}channel_prv_ts;

typedef struct channel_outp_s
{
  int16_t out_perml_s16; // Channel value [0.1 %]

}channel_outp_ts;

typedef struct channel_s
{
  channel_inp_ts  inp_s;
  channel_prm_ts  prm_s;
  channel_prv_ts  prv_s;
  channel_outp_ts outp_s;

}channel_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

CHANNEL_SCOPE uint8_t channel_init( channel_ts * inst_ps );
CHANNEL_SCOPE void channel( channel_ts * inst_ps );

#endif /* CHANNEL_H_ */
