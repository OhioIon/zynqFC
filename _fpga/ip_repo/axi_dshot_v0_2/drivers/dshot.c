#define DSHOT_C

/** \file  dshot.c
 *  \brief dshot device driver
 *
 *  \author OhioIon 2019
 *
 */

/****************** Includes ********************/

#include "dshot.h"

/******************* Defines ********************/

#define REG0_D  0x0 // Register 0 - Control register
#define REG1_D  0x4 // Register 1 - Status register

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static inline void WrReg32(volatile uint32_t *p, volatile uint32_t d){*p = d;} // Write 32-bit word to register space
static inline volatile uint32_t RdReg32(volatile uint32_t *p){return *p;}      // Read 32-bit word from register space

/**************** Implementation ****************/


/** \brief Initialize DShot driver instance
 *
 * \param  ps: Driver instance pointer
 * \return 0 ok, 1 error
 *
 */
bool dshot_init( dshot_ts *ps )
{
  // Pointer check
  if( ps == NULL ) return 1;

  // Parameter Check
  if( ps->prm_s.addrBas_pv == NULL ) return 1;

  // Init privates/outputs
  ps->outp_s.stErr_u8 = 0U;

  // Init success
  ps->prv_s.flgIni_l = 1;

  return 0; // OK
}


/** \brief Send DShot packet
 *
 * Assembles and sends a DShot packet towards ESC. Sending
 * can fail in case previous transmission is busy.
 *
 * \param  ps: Driver instance pointer
 * \return -
 *
 */
void dshot( dshot_ts *ps )
{
  // Pointer check
  if( ps == NULL ) return;

  // Init check
  if( !ps->prv_s.flgIni_l ) return;

  // Reset error symptoms
  ps->outp_s.stErr_u8 = 0U;

  // Read reg1 to check if transmission is busy
  uint32_t reg_u32 = RdReg32( ps->prm_s.addrBas_pv + REG1_D );
  if( reg_u32 != 0U )
  {
    // Output busy symptom
    ps->outp_s.stErr_u8 |= DSHOT_ErrBusy_DU8;
    return;
  }

  // Limit inputs
  dshot_inp_ts inp_s = ps->inp_s;
  if( inp_s.thrData_u16 > 2047U ) inp_s.thrData_u16 = 2047U;
  if( inp_s.flgReqTel_u8 > 1U ) inp_s.flgReqTel_u8 = 1U;

  // Prepare data word (without CRC)
  uint16_t dat_u16;
  dat_u16  = inp_s.thrData_u16  << 5;
  dat_u16 |= inp_s.flgReqTel_u8 << 4;

  // Calculate CRC (XOR of data nibbles)
  uint16_t crc_u16;
  crc_u16  = dat_u16 >>  4;
  crc_u16 ^= dat_u16 >>  8;
  crc_u16 ^= dat_u16 >> 12;
  crc_u16 &= 0x000F;

  // Combine data/CRC to 16-bit packet
  dat_u16 |= crc_u16;

  // Write data to reg0 (automatically starts DShot transmission)
  WrReg32( ps->prm_s.addrBas_pv + REG0_D, dat_u16 );
}

// EOF
