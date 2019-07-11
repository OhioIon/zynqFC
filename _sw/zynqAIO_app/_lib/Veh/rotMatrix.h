#ifndef ROTMATRIX_H_
#define ROTMATRIX_H_

#ifdef ROTMATRIX_C
#define ROTMATRIX_SCOPE
#else
#define ROTMATRIX_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct rotMatrix_inp_s
{
  int16_t yaw_s16;
  int16_t pit_s16;
  int16_t rol_s16;

}rotMatrix_inp_ts;

typedef struct rotMatrix_prm_s
{
  float matrix_f[3][3];

}rotMatrix_prm_ts;

typedef struct rotMatrix_outp_s
{
  int16_t yaw_s16;
  int16_t pit_s16;
  int16_t rol_s16;

}rotMatrix_outp_ts;

typedef struct rotMatrix_s
{
  rotMatrix_inp_ts  inp_s;
  rotMatrix_prm_ts  prm_s;
  rotMatrix_outp_ts outp_s;

}rotMatrix_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

ROTMATRIX_SCOPE uint8_t rotMatrix_init( rotMatrix_ts * inst_ps );
ROTMATRIX_SCOPE void rotMatrix( rotMatrix_ts * inst_ps );

#endif /* ROTMATRIX_H_ */
