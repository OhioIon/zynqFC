#define CHANNEL_C

/****************** Includes ********************/

#include "channel.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static void channel_PosNeg( channel_ts * ps );
static void channel_PosOnly( channel_ts * ps );

/**************** Implementation ****************/

// Channel initialization
uint8_t channel_init( channel_ts * ps )
{
  // Initialize privates

  // Initialize outputs
  ps->outp_s.out_perml_s16 = 0;

  // Done
  return 0;
} 

// Channel processing
void channel( channel_ts * ps )
{
  if( ps->prm_s.flgPosOnly_u8 == 0 )
  {
    channel_PosNeg( ps );
  }
  else
  {
    channel_PosOnly( ps );
  }
}

// Normal channel. Output -1000 ... 1000
static void channel_PosNeg( channel_ts * ps )
{
  uint8_t flgNeg_u8 = 0;

  // Calculate mid-point in curve (output value 0)
  uint16_t mid_us_u16 = (ps->prm_s.hi_us_u16 - ps->prm_s.lo_us_u16) / 2 + ps->prm_s.lo_us_u16;

  // Calculate position in curve
  double pos_lf;
  if( ps->inp_s.in_us_u16 >= mid_us_u16 )
  {
    pos_lf = (double)(ps->inp_s.in_us_u16 - mid_us_u16) / (double)(ps->prm_s.hi_us_u16 - mid_us_u16);
  }
  else
  {
    pos_lf = (double)(mid_us_u16 - ps->inp_s.in_us_u16) / (double)(mid_us_u16 - ps->prm_s.lo_us_u16);
    flgNeg_u8 = 1; // keep in mind that output is negative
  }

  // Limit to 100 %
  if( pos_lf >  1.0 ) pos_lf =  1.0;

  // Apply dead-band
  double deadBand_lf = (double)ps->prm_s.deadBand_perml_u16 / 1000.0;
  if( pos_lf < deadBand_lf )
  {
    pos_lf = 0.0;
  }
  else
  {
    pos_lf = (pos_lf - deadBand_lf) / (1.0 - deadBand_lf);
  }

  // Apply sign
  if( flgNeg_u8 != 0 ) pos_lf = -pos_lf;

  // Reverse direction
  if( ps->prm_s.flgReverse_u8 != 0 ) pos_lf = -pos_lf;

  // Convert to integer
  ps->outp_s.out_perml_s16 = (int16_t)(pos_lf * 1000.0);

  // Limit output
  if( ps->outp_s.out_perml_s16 >  1000 ) ps->outp_s.out_perml_s16 =  1000;
  if( ps->outp_s.out_perml_s16 < -1000 ) ps->outp_s.out_perml_s16 = -1000;
}

// Only positive channel. Output 0 ... 1000
static void channel_PosOnly( channel_ts * ps )
{
  // Calculate position in curve
  double pos_lf = (double)(ps->inp_s.in_us_u16 - ps->prm_s.lo_us_u16) / (double)(ps->prm_s.hi_us_u16 - ps->prm_s.lo_us_u16);

  // Reverse direction
  if( ps->prm_s.flgReverse_u8 ) pos_lf = 1.0 - pos_lf;

  // Prevent negative value
  if( pos_lf < 0 ) pos_lf = 0.0;

  // Apply dead-band
  double deadBand_lf = (double)ps->prm_s.deadBand_perml_u16 / 1000.0;
  if( pos_lf < deadBand_lf )
  {
    pos_lf = 0.0;
  }
  else
  {
    pos_lf = (pos_lf - deadBand_lf) / (1.0 - deadBand_lf);
  }

  // Convert to integer
  ps->outp_s.out_perml_s16 = (int16_t)(pos_lf * 1000.0);

  // Limit output
  if( ps->outp_s.out_perml_s16 > 1000 ) ps->outp_s.out_perml_s16 = 1000;
}

// EOF

