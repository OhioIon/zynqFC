#define EXPO_C

/****************** Includes ********************/

#include "expo.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Exponential function initialization
uint8_t expo_init( expo_ts * inst_ps )
{
  // Initialize privates

  // Initialize outputs
  inst_ps->outp_s.out_perml_s16 = 0;

  // Done
  return 0;
} 

// Signal processing
void expo( expo_ts * inst_ps )
{
  // Limit expo to max 100 percent
  uint8_t expo_perc_u8 = inst_ps->prm_s.expo_perc_u8;
  if( expo_perc_u8 > 100 ) expo_perc_u8 = 100;

  // Calculate interpolation between linear (y = x) and
  // exponential (y = x ^ 3) transfer function
  double in_lf  = (double)inst_ps->inp_s.in_perml_s16;
  double exp_lf = (double)expo_perc_u8;
  double out_lf = ((100.0 - exp_lf) / 100.0) * in_lf + (exp_lf / 100.0) * (in_lf * in_lf * in_lf / 1000000.0);

  // Convert back to integer
  inst_ps->outp_s.out_perml_s16 = (int16_t)out_lf;

  // Limit output
  if( inst_ps->outp_s.out_perml_s16 >  1000 ) inst_ps->outp_s.out_perml_s16 =  1000;
  if( inst_ps->outp_s.out_perml_s16 < -1000 ) inst_ps->outp_s.out_perml_s16 = -1000;
}

// EOF
