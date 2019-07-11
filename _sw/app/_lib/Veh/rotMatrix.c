#define ROTMATRIX_C

/****************** Includes ********************/

#include "rotMatrix.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Exponential function initialization
uint8_t rotMatrix_init( rotMatrix_ts * inst_ps )
{
  // Initialize privates

  // Initialize outputs
  inst_ps->outp_s.yaw_s16 = 0;
  inst_ps->outp_s.pit_s16 = 0;
  inst_ps->outp_s.rol_s16 = 0;

  // Done
  return 0;
} 

// Signal processing
void rotMatrix( rotMatrix_ts * inst_ps )
{
  double yaw_lf;
  double pit_lf;
  double rol_lf;

  // Matrix multiplication
  yaw_lf  = inst_ps->inp_s.yaw_s16 * inst_ps->prm_s.matrix_f[0][0];
  yaw_lf += inst_ps->inp_s.pit_s16 * inst_ps->prm_s.matrix_f[0][1];
  yaw_lf += inst_ps->inp_s.rol_s16 * inst_ps->prm_s.matrix_f[0][2];
  pit_lf  = inst_ps->inp_s.yaw_s16 * inst_ps->prm_s.matrix_f[1][0];
  pit_lf += inst_ps->inp_s.pit_s16 * inst_ps->prm_s.matrix_f[1][1];
  pit_lf += inst_ps->inp_s.rol_s16 * inst_ps->prm_s.matrix_f[1][2];
  rol_lf  = inst_ps->inp_s.yaw_s16 * inst_ps->prm_s.matrix_f[2][0];
  rol_lf += inst_ps->inp_s.pit_s16 * inst_ps->prm_s.matrix_f[2][1];
  rol_lf += inst_ps->inp_s.rol_s16 * inst_ps->prm_s.matrix_f[2][2];

  // Convert to integer and output
  inst_ps->outp_s.yaw_s16 = yaw_lf;
  inst_ps->outp_s.pit_s16 = pit_lf;
  inst_ps->outp_s.rol_s16 = rol_lf;

}

// EOF
