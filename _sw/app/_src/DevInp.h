#ifndef DEVINP_H
#define DEVINP_H

#ifdef DEVINP_C
#define DEVINP_SCOPE
#else
#define DEVINP_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

#include "nrf24l01.h"
#include "mpu6000.h"
#include "et6i.h"
#include "maf.h"
#include "rotMatrix.h"
#include "channel.h"

/******************* Defines ********************/

/******************** Types *********************/

typedef struct DevInp_outp_s
{
  // Remote Control Status
  uint8_t flgCon_u8;   // Remote control connected
  uint8_t flgLost_u8;  // Signal lost
  uint8_t flgStart_u8; // Start condition detected

  // Remote Control Axis Data [‰]
  int16_t yaw_perml_s16;
  int16_t pit_perml_s16;
  int16_t rol_perml_s16;
  int16_t thr_perml_s16;

  // Rate of rotation [0.1 °/s]
  int16_t yaw_p1degps_s16;
  int16_t pit_p1degps_s16;
  int16_t rol_p1degps_s16;

}DevInp_outp_ts;

typedef struct DevInp_s
{
  DevInp_outp_ts outp_s;

  // Moving average instances
  maf_ts mafZ_s;
  maf_ts mafX_s;
  maf_ts mafY_s;

  // Rotation matrix instance
  rotMatrix_ts rotMatrix_s;

  // Channel instances
  channel_ts channelYaw_s;
  channel_ts channelPit_s;
  channel_ts channelRol_s;
  channel_ts channelThr_s;

}DevInp_ts;

/****************** Public Data *****************/

DEVINP_SCOPE DevInp_ts DevInp_s;

/******************* Interface ******************/

DEVINP_SCOPE uint8_t DevInp_init( void );
DEVINP_SCOPE void DevInp( void );

#endif // DEVINP_H
