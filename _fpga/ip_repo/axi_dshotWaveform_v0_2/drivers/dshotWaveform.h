#ifndef DSHOTWAVEFORM_H
#define DSHOTWAVEFORM_H

#ifdef DSHOTWAVEFORM_C
#define DSHOTWAVEFORM_SCOPE
#else
#define DSHOTWAVEFORM_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct dshotWaveform_prm_s
{
  void *addrBas_pv;        // Physical base address of registers
  uint16_t speed_kbps_u16; // DShot protocol speed 150 till 2400 (e.g. 600 = DShot600 = 600 kbps)
}dshotWaveform_prm_ts;

typedef struct dshotWaveform_s
{
  dshotWaveform_prm_ts  prm_s;
}dshotWaveform_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

DSHOTWAVEFORM_SCOPE uint8_t dshotWaveform_init(dshotWaveform_ts *ps );

#endif // DSHOTWAVEFORM_H
