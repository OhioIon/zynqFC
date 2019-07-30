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

/******************* Defines ********************/

/******************** Types *********************/

typedef struct DevInp_outp_s
{
  // Remote Control Status
  uint8_t flgCon_u8;   // Remote control connected
  uint8_t flgLost_u8;  // Signal lost

  // Remote Control Axis Data [us]
  uint16_t yaw_us_u16;
  uint16_t pit_us_u16;
  uint16_t rol_us_u16;
  uint16_t thr_us_u16;

  // Rate of rotation [°/s]
  int16_t yaw_degps_s16;
  int16_t pit_degps_s16;
  int16_t rol_degps_s16;

}DevInp_outp_ts;

typedef struct DevInp_s
{
  DevInp_outp_ts outp_s;

}DevInp_ts;

/****************** Public Data *****************/

DEVINP_SCOPE DevInp_ts DevInp_s;

/******************* Interface ******************/

DEVINP_SCOPE uint8_t DevInp_init( void );
DEVINP_SCOPE void DevInp( void );

#endif // DEVINP_H
