#define ARM_C

/****************** Includes ********************/

#include "arm.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static uint8_t arm_checkSig( int16_t ch_perml_s16, int16_t thres_perml_s16 );
static void arm_deb( arm_ts * inst_ps );

/**************** Implementation ****************/

// ARM initialization
uint8_t arm_init( arm_ts * inst_ps )
{
  // Init privates
  inst_ps->prv_s.flgYaw_u8    = 0;
  inst_ps->prv_s.flgPit_u8    = 0;
  inst_ps->prv_s.flgRol_u8    = 0;
  inst_ps->prv_s.flgThr_u8    = 0;
  inst_ps->prv_s.flgArmed_u8  = 0;
  inst_ps->prv_s.cntArmed_us_u32 = 0;

  // Init outputs
  inst_ps->outp_s.flgArmed_u8 = 0;

  // Done
  return 0;
} 

// ARM detection
void arm( arm_ts * inst_ps )
{
  // Process as long as arm is not reached
  // Process only in case remote control is connected
  if( (0 == inst_ps->outp_s.flgArmed_u8) && (0 != inst_ps->inp_s.flgCon_u8) )
  {
    // Check signals against thresholds
    inst_ps->prv_s.flgYaw_u8 = arm_checkSig( inst_ps->inp_s.yaw_perml_s16, inst_ps->prm_s.yawThres_perml_s16);
    inst_ps->prv_s.flgPit_u8 = arm_checkSig( inst_ps->inp_s.pit_perml_s16, inst_ps->prm_s.pitThres_perml_s16);
    inst_ps->prv_s.flgRol_u8 = arm_checkSig( inst_ps->inp_s.rol_perml_s16, inst_ps->prm_s.rolThres_perml_s16);
    inst_ps->prv_s.flgThr_u8 = (0 == inst_ps->inp_s.thr_perml_s16);

    // Check if arm condition is reached
    uint8_t sum_u8;
    sum_u8  = inst_ps->prv_s.flgYaw_u8;
    sum_u8 += inst_ps->prv_s.flgPit_u8;
    sum_u8 += inst_ps->prv_s.flgRol_u8;
    sum_u8 += inst_ps->prv_s.flgThr_u8;
    if( sum_u8 == 4 )
    {
      inst_ps->prv_s.flgArmed_u8 = 1;
    }
    else
    {
      inst_ps->prv_s.flgArmed_u8 = 0;
    }

    // Perform debouncing of signal
    arm_deb( inst_ps );
  }
}

// Check channel signal against threshold
static uint8_t arm_checkSig( int16_t ch_perml_s16, int16_t thres_perml_s16 )
{
  uint8_t retVal_u8 = 0;
  if( thres_perml_s16 >= 0 )
  {
    if( ch_perml_s16 >= thres_perml_s16 )
    {
      retVal_u8 = 1;
    }
  }
  else
  {
    if( ch_perml_s16 <= thres_perml_s16 )
    {
      retVal_u8 = 1;
    }
  }

  return retVal_u8;
}

// Debounce arm signal
static void arm_deb( arm_ts * inst_ps )
{
  // Reset condition
  if( 0 == inst_ps->prv_s.flgArmed_u8 )
  {
    inst_ps->prv_s.cntArmed_us_u32 = 0;
  }
  else
  {
    // Debounce counter
    inst_ps->prv_s.cntArmed_us_u32 += inst_ps->prm_s.tiCyc_us_u16;
    if( inst_ps->prv_s.cntArmed_us_u32 >= (uint32_t)inst_ps->prm_s.tiArmDly_ms_u16 * 1000 )
    {
      /////////////// ARMED!!! ///////////////
      inst_ps->outp_s.flgArmed_u8 = 1;
    }
  }
}

// EOF
