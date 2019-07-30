#ifndef VEH_H
#define VEH_H

#ifdef VEH_C
#define VEH_SCOPE
#else
#define VEH_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

#include "channel.h"
#include "arm.h"
#include "expo.h"
#include "rateDes.h"
#include "pid.h"
#include "rotMatrix.h"

/******************* Defines ********************/

/******************** Types *********************/

typedef struct Veh_outp_s
{
  // Armed state
  uint8_t flgArmed_u8;

  // DShot motor control data
  uint16_t rawDataFrntLeft_u16;
  uint16_t rawDataFrntRght_u16;
  uint16_t rawDataRearLeft_u16;
  uint16_t rawDataRearRght_u16;

}Veh_outp_ts;

typedef struct Veh_s
{
  Veh_outp_ts outp_s;

  // Channel instances
  channel_ts channelYaw_s;
  channel_ts channelPit_s;
  channel_ts channelRol_s;
  channel_ts channelThr_s;

  // Rotation matrix instance
  rotMatrix_ts rotMatrix_s;

  // Arm instance
  arm_ts arm_s;

  // Exponential instances
  expo_ts expoYaw_s;
  expo_ts expoPit_s;
  expo_ts expoRol_s;

  // Rate desired instances
  rateDes_ts rateYaw_s;
  rateDes_ts ratePit_s;
  rateDes_ts rateRol_s;

  // PID controller instances
  pid_ts pidYaw_s;
  pid_ts pidPit_s;
  pid_ts pidRol_s;

}Veh_ts;

/****************** Public Data *****************/

VEH_SCOPE Veh_ts Veh_s;

/******************* Interface ******************/

VEH_SCOPE uint8_t Veh_init( void );
VEH_SCOPE void Veh( void );

#endif // VEH_H
