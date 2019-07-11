#ifndef DEVOUTP_H
#define DEVOUTP_H

#ifdef DEVOUTP_C
#define DEVOUTP_SCOPE
#else
#define DEVOUTP_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

#include "dshot.h"
#include "dshotWaveform.h"

/******************* Defines ********************/

/******************** Types *********************/

typedef struct DevOutp_s
{
  // DSHOT waveform generator (determines DSHOT speed)
  dshotWaveform_ts dshotWaveform_s;

  // DSHOT protocol to ESCs of BLDC motors
  dshot_ts dshotMotFrntLeft_s;
  dshot_ts dshotMotFrntRght_s;
  dshot_ts dshotMotRearLeft_s;
  dshot_ts dshotMotRearRght_s;

}DevOutp_ts;

/****************** Public Data *****************/

DEVOUTP_SCOPE DevOutp_ts DevOutp_s;

/******************* Interface ******************/

DEVOUTP_SCOPE uint8_t DevOutp_init( void );
DEVOUTP_SCOPE void DevOutp( void );

#endif // DEVOUTP_H
