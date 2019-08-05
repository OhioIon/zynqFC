#define MAF_C

/****************** Includes ********************/

#include <maf.h>

#include <string.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Exponential function initialization
uint8_t maf_init( maf_ts * ps )
{
  // Initialize privates
  memset( &ps->prv_s, 0, sizeof(ps->prv_s) );

  // Initialize outputs
  memset( &ps->outp_s, 0, sizeof(ps->outp_s) );

  // Done
  return 0;
} 

// Signal processing
void maf( maf_ts * ps )
{
  if ( ps->inp_s.flgEna_u8 )
  {
    // Store current value
    ps->prv_s.maf_as16[ps->prv_s.idx_u16] = ps->inp_s.x_s16;

    // Handle index
    ps->prv_s.idx_u16++;
    if( ps->prv_s.idx_u16 >= MAF_SIZE_D )
    {
      ps->prv_s.idx_u16 = 0;
    }

    // Calculate average
    int32_t sum_s32 = 0;
    int16_t idx_u16;
    for( idx_u16 = 0; idx_u16 < MAF_SIZE_D; idx_u16++ )
    {
      sum_s32 +=  ps->prv_s.maf_as16[idx_u16];
    }
    ps->outp_s.y_s16 = sum_s32 / MAF_SIZE_D;
  }
}

// EOF
