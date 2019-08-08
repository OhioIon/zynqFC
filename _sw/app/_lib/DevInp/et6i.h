#ifndef ET6I_H_
#define ET6I_H_

#ifdef ET6I_C
#define ET6I_SCOPE
#else
#define ET6I_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

#include "nrf24l01.h"

/******************* Defines ********************/

/******************** Types *********************/

typedef enum et6i_stMac_e
{
  et6i_stMacInit_E,
  et6i_stMacWaitForConfig_E,
  et6i_stMacWaitForBinding_E,
  et6i_stMacBindingAbort_E,
  et6i_stMacConfigRxData_E,
  et6i_stMacWaitForReconfig_E,
  et6i_stMacScanForRxData_E,
  et6i_stMacRx_E,
} et6i_stMac_te;

typedef struct et6i_prm_s
{
  uint16_t tiCyc_us_u16;    // Loop/cycle time [µs]
  uint16_t timeoutRx_ms_u8; // RX timeout [ms]

}et6i_prm_ts;

typedef struct et6i_prv_s
{
  et6i_stMac_te st_e;
  nrf24l01_cfg_ts cfgTrx_s;
  uint8_t addr_au8[3];

  uint8_t bandInfo_u8;
  uint8_t strtBand1_u8;
  uint8_t strtBand2_u8;
  uint8_t currCh1_u8;
  uint8_t currCh2_u8;

  uint32_t cnt_u32;
  uint32_t cntTimeout_u32;

  uint32_t cntPktCh1_u32;
  uint32_t cntPktCh2_u32;

}et6i_prv_ts;

typedef struct et6i_outp_s
{
  uint8_t flgBind_u8; // Binding is in progress
  uint8_t flgCon_u8;  // Connected / valid channel data
  uint8_t flgLost_u8; // Lost signal / timeout error

  uint16_t ch1_us_u16;
  uint16_t ch2_us_u16;
  uint16_t ch3_us_u16;
  uint16_t ch4_us_u16;
  uint16_t ch5_us_u16;
  uint16_t ch6_us_u16;

}et6i_outp_ts;

typedef struct et6i_s
{
  et6i_prm_ts  prm_s;
  et6i_prv_ts  prv_s;
  et6i_outp_ts outp_s;

}et6i_ts;

/****************** Public Data *****************/

ET6I_SCOPE et6i_ts et6i_s;

/******************* Interface ******************/

ET6I_SCOPE uint8_t et6i_init( void );
ET6I_SCOPE void et6i( void );

#endif /* ET6I_H_ */
