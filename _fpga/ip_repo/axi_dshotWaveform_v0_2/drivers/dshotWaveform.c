#define DSHOTWAVEFORM_C

/****************** Includes ********************/

#include "dshotWaveform.h"

/******************* Defines ********************/
#define REG0   0
#define REG1   4
#define REG2   8
#define ENABLE 0x80000000

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

static inline void WrReg32(volatile uint32_t * p, volatile uint32_t d);
static inline volatile uint32_t RdReg32(volatile uint32_t * p);

/**************** Implementation ****************/


/**
 * @brief Initialize DShot Waveform generation peripheral driver instance
 *
 * The waveform generated by this module is a cyclic repetition of the
 * 0-bit and 1-bit pattern of the DShot protocol. This is used by the
 * DShot peripheral.
 *
 * @return 0 success, != 0 failure
 */
uint8_t dshotWaveform_init( dshotWaveform_ts *ps )
{
  uint8_t retVal_u8 = 0;
  if( ps != NULL )
  {
    // Parameter Check
    if( (ps->prm_s.addrBas_pv != NULL) && (ps->prm_s.speed_kbps_u16 >= 150) && (ps->prm_s.speed_kbps_u16 <= 2400) )
    {
      // Read AXI4-lite clock frequency from reg2
      uint32_t frqClk_Hz_u32 = RdReg32( ps->prm_s.addrBas_pv + REG2 );

      if( frqClk_Hz_u32 != 0U )
      {
        // Calculate total bit-time (round)
        uint32_t cntTot_u32 = (frqClk_Hz_u32 + (uint32_t)ps->prm_s.speed_kbps_u16 * 500U) / ((uint32_t)ps->prm_s.speed_kbps_u16 * 1000U);

        // Calculate high-time for bit pattern 1 (round)
        uint32_t cntT1H_u32 = (uint32_t)(0.748503f * cntTot_u32);

        // Calculate high-time for bit pattern 0 (floor)
        uint32_t cntT0H_u32 = cntT1H_u32 / 2;

        // Make some final checks
        if( (cntT0H_u32 < cntT1H_u32) &&
            (cntT1H_u32 < cntTot_u32) &&
            (cntTot_u32 < 0xFFFF    ) )
        {
          // Write high/low counts to reg1
          WrReg32( ps->prm_s.addrBas_pv + REG1, (cntT1H_u32 << 16) | cntT0H_u32 );

          // Write total count to reg0 and enable continuous waveform generation
          WrReg32( ps->prm_s.addrBas_pv + REG0, (cntTot_u32 - 1) | ENABLE );
        }
        else
        {
          retVal_u8++;
        }
      }
      else
      {
        retVal_u8++;
      }
    }
    else
    {
      retVal_u8++;
    }
  }
  else
  {
    retVal_u8++;
  }

  return retVal_u8;
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
