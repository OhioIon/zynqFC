#ifndef DSHOTWAVEFORM_H
#define DSHOTWAVEFORM_H

/** \file  dshotWaveform.h
 *  \brief dshotWaveform device driver
 *
 *  The dshotWaveform IP-core is programmed in VHDL and synthesized into the FPGA fabric.
 *  It is connected to the CPU via AXI4-Lite bus.
 *
 *  Once initialized with this driver, the IP-core continuously generates
 *  high-bit and low-bit patterns of the DShot protocol. The generated
 *  pattern waveforms are finally used by "dshot" IP-core to send DShot
 *  data packets to electronic speed controllers (ESCs).
 *
 *  The "dshotWaveform" IP-core hence controls the transmission speed (bit-rate)
 *  of the DShot communication while the "dshot" IP-core controls the data packet
 *  transmission and contents.
 *
 *  \author OhioIon 2019
 *
 */

#ifdef DSHOTWAVEFORM_C
#define DSHOTWAVEFORM_SCOPE
#else
#define DSHOTWAVEFORM_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/******************* Defines ********************/

/******************** Types *********************/

/// dshotWaveform parameters
typedef struct dshotWaveform_prm_s
{
  void     *addrBas_pv;    ///< Physical base address of IP-core registers
  uint16_t speed_kbps_u16; ///< Desired DShot protocol speed (150 - 2400). Example: 600 = DShot600 = 600 kbps

}dshotWaveform_prm_ts;

/// dshotWaveform driver instance
typedef struct dshotWaveform_s
{
  dshotWaveform_prm_ts  prm_s; ///< parameters

}dshotWaveform_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

DSHOTWAVEFORM_SCOPE bool dshotWaveform_init(dshotWaveform_ts *ps );

#endif // DSHOTWAVEFORM_H
