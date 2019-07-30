#define LED_C

/****************** Includes ********************/

#include "led.h"

#include <string.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize LED instance
uint8_t led_init( led_ts *inst_ps )
{
  // Initialize privates
  memset( &inst_ps->prv_s, 0, sizeof(inst_ps->prv_s) );

  // Calculate blinking counter limits (half of period)
  inst_ps->prv_s.limBlinkSlow_u32 = (uint32_t)inst_ps->prm_s.tiBlinkSlow_ms_u16 * 500 / inst_ps->prm_s.tiCyc_us_u16;
  inst_ps->prv_s.limBlinkFast_u32 = (uint32_t)inst_ps->prm_s.tiBlinkFast_ms_u16 * 500 / inst_ps->prm_s.tiCyc_us_u16;

  // Initialize outputs
  memset( &inst_ps->outp_s, 0, sizeof(inst_ps->outp_s) );

  // Done
  return 0;
} 

// Handle LED mode (off, blink-slow, blink-fast, on)
void led( led_ts *inst_ps )
{
  // Mode selection
  switch( inst_ps->inp_s.fct_e )
  {
  case led_fctOn_E: // LED should be ON

    // Reset counter
    inst_ps->prv_s.cnt_u32 = 0;

    // Enable output
    inst_ps->outp_s.flgOn_u8 = 1;

    break;

  case led_fctBlinkSlow_E: // LED should blink slow

    // Check if counter reached limit
    if( inst_ps->prv_s.cnt_u32 >= inst_ps->prv_s.limBlinkSlow_u32 )
    {
      // Switch output polarity
      inst_ps->outp_s.flgOn_u8 = !inst_ps->outp_s.flgOn_u8;

      // Reset count
      inst_ps->prv_s.cnt_u32 = 0;
    }
    else
    {
      // Count cycles
      inst_ps->prv_s.cnt_u32++;
    }

    break;

  case led_fctBlinkFast_E: // LED should blink fast

    // Check if counter reached limit
    if( inst_ps->prv_s.cnt_u32 >= inst_ps->prv_s.limBlinkFast_u32 )
    {
      // Switch output polarity
      inst_ps->outp_s.flgOn_u8 = !inst_ps->outp_s.flgOn_u8;

      // Reset count
      inst_ps->prv_s.cnt_u32 = 0;
    }
    else
    {
      // Count cycles
      inst_ps->prv_s.cnt_u32++;
    }

    break;

  default: // LED should be OFF

    // Reset counter
    inst_ps->prv_s.cnt_u32 = 0;

    // Disable output
    inst_ps->outp_s.flgOn_u8 = 0;

    break;

  }
}

// EOF

