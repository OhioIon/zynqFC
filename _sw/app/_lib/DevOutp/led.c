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
uint8_t led_init( led_ts *ps )
{
  // Initialize privates
  memset( &ps->prv_s, 0, sizeof(ps->prv_s) );

  // Calculate blinking counter limits (half of period)
  ps->prv_s.limBlinkSlow_u32 = (uint32_t)ps->prm_s.tiBlinkSlow_ms_u16 * 500 / ps->prm_s.tiCyc_us_u16;
  ps->prv_s.limBlinkFast_u32 = (uint32_t)ps->prm_s.tiBlinkFast_ms_u16 * 500 / ps->prm_s.tiCyc_us_u16;

  // Initialize outputs
  memset( &ps->outp_s, 0, sizeof(ps->outp_s) );

  // Done
  return 0;
} 

// Handle LED mode (off, blink-slow, blink-fast, on)
void led( led_ts *ps )
{
  // Mode selection
  switch( ps->inp_s.fct_e )
  {
  case led_fctOn_E: // LED should be ON

    // Reset counter
    ps->prv_s.cnt_u32 = 0;

    // Enable output
    ps->outp_s.flgOn_u8 = 1;

    break;

  case led_fctBlinkSlow_E: // LED should blink slow

    // Check if counter reached limit
    if( ps->prv_s.cnt_u32 >= ps->prv_s.limBlinkSlow_u32 )
    {
      // Switch output polarity
      ps->outp_s.flgOn_u8 = !ps->outp_s.flgOn_u8;

      // Reset count
      ps->prv_s.cnt_u32 = 0;
    }
    else
    {
      // Count cycles
      ps->prv_s.cnt_u32++;
    }

    break;

  case led_fctBlinkFast_E: // LED should blink fast

    // Check if counter reached limit
    if( ps->prv_s.cnt_u32 >= ps->prv_s.limBlinkFast_u32 )
    {
      // Switch output polarity
      ps->outp_s.flgOn_u8 = !ps->outp_s.flgOn_u8;

      // Reset count
      ps->prv_s.cnt_u32 = 0;
    }
    else
    {
      // Count cycles
      ps->prv_s.cnt_u32++;
    }

    break;

  default: // LED should be OFF

    // Reset counter
    ps->prv_s.cnt_u32 = 0;

    // Disable output
    ps->outp_s.flgOn_u8 = 0;

    break;

  }
}

// EOF

