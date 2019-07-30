#ifndef RATEDES_H_
#define RATEDES_H_

#ifdef RATEDES_C
#define RATEDES_SCOPE
#else
#define RATEDES_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct rateDes_inp_s
{
  int16_t in_perml_s16; // Channel value [0.1 %]

}rateDes_inp_ts;

typedef struct rateDes_prm_s
{
  uint16_t rateMax_degps_u16; // RC rate on max. stick deflection [°/s]

}rateDes_prm_ts;

typedef struct rateDes_outp_s
{
  int16_t rate_degps_s16; // Desired rotation rate setpoint [°/s]

}rateDes_outp_ts;

typedef struct rateDes_s
{
  rateDes_inp_ts  inp_s;
  rateDes_prm_ts  prm_s;
  rateDes_outp_ts outp_s;

}rateDes_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

RATEDES_SCOPE uint8_t rateDes_init( rateDes_ts * ps );
RATEDES_SCOPE void rateDes( rateDes_ts * ps );

#endif /* RATEDES_H_ */
