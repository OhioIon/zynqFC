#define DSHOT_C

/****************** Includes ********************/

#include "dshot.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static inline void WrReg32(volatile uint32_t * p, volatile uint32_t d);
static inline volatile uint32_t RdReg32(volatile uint32_t * p);

/**************** Implementation ****************/


/**
 * @brief Initialize DShot peripheral driver instance
 *
 * @return 0 success, != 0 failure
 */
uint8_t dshot_init( dshot_ts *ps )
{
  uint8_t retVal_u8 = 0;
  if( ps != NULL )
  {
    // Check parameters
    if( ps->prm_s.addrBas_pv == NULL ) retVal_u8++;

    // Init privates/outputs
    ps->outp_s.stErr_u8 = 0U;
  }
  else
  {
    retVal_u8++;
  }

  return retVal_u8;
}

/**
 * @brief Initiates a single DShot data transmission to ESC
 */
void dshot( dshot_ts *ps )
{
  if( ps != NULL )
  {
    // Reset error symptoms
    ps->outp_s.stErr_u8 = 0U;

    // Limit inputs
    dshot_inp_ts inp_s = ps->inp_s;
    if( inp_s.rawData_u16 > 2047U ) inp_s.rawData_u16 = 2047U;
    if( inp_s.flgReqTel_u8 > 1U ) inp_s.flgReqTel_u8 = 1U;

    // Prepare data word without CRC
    uint16_t dat_u16;
    dat_u16  = inp_s.rawData_u16  << 5;
    dat_u16 |= inp_s.flgReqTel_u8 << 4;

    // Calculate CRC (XOR of data nibbles)
    uint16_t crc_u16;
    crc_u16  = dat_u16 >>  4;
    crc_u16 ^= dat_u16 >>  8;
    crc_u16 ^= dat_u16 >> 12;
    crc_u16 &= 0x000F;

    // Combine data/CRC to 16-bit value
    dat_u16 |= crc_u16;

    // Read reg1 to check if transmission is busy
    uint32_t reg_u32 = RdReg32( ps->prm_s.addrBas_pv + 4 );
    if( reg_u32 == 0U )
    {
      // Write to reg0 in order to start DShot transmission
      WrReg32( ps->prm_s.addrBas_pv, dat_u16 );
    }
    else
    {
      // Error
      ps->outp_s.stErr_u8 |= DSHOT_ErrBusy_DU8;
    }
  }
}

/**
 * @brief Write 32-bit word to register space
 */
static inline void WrReg32( volatile uint32_t * p, volatile uint32_t d )
{
  *p = d;
}

/**
 * @brief Read 32-bit word from register space
 */
static inline volatile uint32_t RdReg32( volatile uint32_t * p )
{
  return *p;
}

// EOF
