#ifndef EXPO_H_
#define EXPO_H_

#ifdef EXPO_C
#define EXPO_SCOPE
#else
#define EXPO_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct expo_inp_s
{
  int16_t in_perml_s16; // Channel value [0.1 %]

}expo_inp_ts;

typedef struct expo_prm_s
{
  uint8_t expo_perc_u8; // Expo value [%]. Only positve supported (decrease sensitivity around stick center)

}expo_prm_ts;

typedef struct expo_outp_s
{
  int16_t out_perml_s16; // Channel value [0.1 %]

}expo_outp_ts;

typedef struct expo_s
{
  expo_inp_ts  inp_s;
  expo_prm_ts  prm_s;
  expo_outp_ts outp_s;

}expo_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

EXPO_SCOPE uint8_t expo_init( expo_ts * inst_ps );
EXPO_SCOPE void expo( expo_ts * inst_ps );

#endif /* EXPO_H_ */
