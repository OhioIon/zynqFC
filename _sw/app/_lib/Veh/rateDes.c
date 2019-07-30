#define RATEDES_C

/****************** Includes ********************/

#include "rateDes.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Exponential function initialization
uint8_t rateDes_init( rateDes_ts * ps )
{
  // Initialize privates

  // Initialize outputs
  ps->outp_s.rate_degps_s16 = 0;

  // Done
  return 0;
} 

// Signal processing
void rateDes( rateDes_ts * ps )
{
  // Gain function
  double rate_lf = (double)ps->inp_s.in_perml_s16 / 1000.0 * (double)ps->prm_s.rateMax_degps_u16;

  // Convert to integer
  ps->outp_s.rate_degps_s16 = rate_lf;
}

// EOF
