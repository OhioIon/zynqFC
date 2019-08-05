#ifndef MAF_H_
#define MAF_H_

#ifdef MAF_C
#define MAF_SCOPE
#else
#define MAF_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

#define MAF_SIZE_D 64 // Size of moving-average-filter

/******************** Types *********************/

typedef struct maf_inp_s
{
  int16_t x_s16;     // Input
  uint8_t flgEna_u8; // Enable MAF

}maf_inp_ts;

typedef struct maf_prv_s
{
  uint16_t idx_u16;              // Index
  int16_t  maf_as16[MAF_SIZE_D]; // Storage

}maf_prv_ts;

typedef struct maf_outp_s
{
  int16_t y_s16; // Output

}maf_outp_ts;

typedef struct maf_s
{
  maf_inp_ts  inp_s;
  maf_prv_ts  prv_s;
  maf_outp_ts outp_s;

}maf_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

MAF_SCOPE uint8_t maf_init( maf_ts * ps );
MAF_SCOPE void maf( maf_ts * ps );

#endif /* MAF_H_ */
