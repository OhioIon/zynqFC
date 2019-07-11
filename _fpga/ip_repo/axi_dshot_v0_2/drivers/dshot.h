#ifndef DSHOT_H
#define DSHOT_H

/** \file  dshot.h
 *  \brief dshot device driver
 *
 *  The dshot IP-core is programmed in VHDL and synthesized into the FPGA fabric.
 *  It is connected to the CPU via AXI4-Lite bus.
 *
 *  This IP-core allows to send DShot data packets to electronic speed controllers
 *  (ESCs). A DShot packet is 16-bit in total. 11-bit for throttle, 1 bit for
 *  telemetry request (separate return channel), and 4 bit CRC.
 *
 *  A throttle value of 0 indicates disarmed. This value must be send repeatedly
 *  during arming sequence in order to activate/arm ESC.
 *
 *  Normal throttle values use range 48 to 2047. The values 1 - 47 are reserved
 *  (e.g. for startup tones or commands). The value 1024 shall indicate half throttle.
 *
 *  The ESC might fail safe automatically after no DShot packet was received for
 *  some milliseconds. The system design assumes DShot packets are send at the end of
 *  each control loop of the flight controller.
 *
 *  DShot assumes there is >= 2 us delay between
 *  two packets. This needs to be ensured by caller.
 *
 *  While the packet content and transmission time is controlled by this IP-core,
 *  the DShot transmission speed (kbps) is defined by the dshotWaveform IP-core.
 *
 *  \author OhioIon 2019
 *
 */

#ifdef DSHOT_C
#define DSHOT_SCOPE
#else
#define DSHOT_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/******************* Defines ********************/

#define DSHOT_ErrBusy_DU8 (uint8_t)0x01 ///< Error mask: Busy, cannot transmit data

/******************** Types *********************/

/// dshot inputs
typedef struct dshot_inp_s
{
  uint16_t thrData_u16;  ///< Throttle data to ESC (11-bit)
  uint8_t  flgReqTel_u8; ///< Request ESC telemetry (1-bit)
}dshot_inp_ts;

/// dshot parameters
typedef struct dshot_prm_s
{
  void *addrBas_pv; ///< Physical base address of IP-core registers
}dshot_prm_ts;

/// dshot privates
typedef struct dshot_prv_s
{
  bool flgIni_l; ///< Initialization success
}dshot_prv_ts;

/// dshot outputs
typedef struct dshot_outp_s
{
  uint8_t stErr_u8; ///< Error detection bit-mask
}dshot_outp_ts;

/// dshot driver instance
typedef struct dshot_s
{
  dshot_inp_ts  inp_s;  ///< inputs
  dshot_prm_ts  prm_s;  ///< parameters
  dshot_prv_ts  prv_s;  ///< privates
  dshot_outp_ts outp_s; ///< outputs
}dshot_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

DSHOT_SCOPE bool dshot_init(dshot_ts *ps );
DSHOT_SCOPE void dshot(dshot_ts *ps );

#endif // DSHOT_H
