#ifndef DSHOT_H
#define DSHOT_H

#ifdef DSHOT_C
#define DSHOT_SCOPE
#else
#define DSHOT_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

// Error bit-mask(s)
#define DSHOT_ErrBusy_DU8 (uint8_t)0x01 // Busy - cannot transmit data

/******************** Types *********************/

typedef struct dshot_inp_s
{
  uint16_t rawData_u16;  // Throttle data to ESC (11-bit)
  uint8_t  flgReqTel_u8; // Request ESC telemetry (1-bit)
}dshot_inp_ts;

typedef struct dshot_prm_s
{
  void *addrBas_pv; // Physical base address of registers
}dshot_prm_ts;

typedef struct dshot_outp_s
{
  uint8_t stErr_u8; // Error detection flag-mask
}dshot_outp_ts;

typedef struct dshot_s
{
  dshot_inp_ts  inp_s;
  dshot_prm_ts  prm_s;
  dshot_outp_ts outp_s;
}dshot_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

DSHOT_SCOPE uint8_t dshot_init(dshot_ts *ps );
DSHOT_SCOPE void dshot(dshot_ts *ps );

#endif // DSHOT_H
