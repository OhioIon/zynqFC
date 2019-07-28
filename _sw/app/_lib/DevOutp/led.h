#ifndef LED_H_
#define LED_H_

#ifdef LED_C
#define LED_SCOPE
#else
#define LED_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef enum led_stMac_e
{
  led_fctOff_E = 0,   // LED Off
  led_fctBlinkSlow_E, // LED blinks with slow period
  led_fctBlinkFast_E, // LED blinks with fast period
  led_fctOn_E,        // LED on
} led_fct_te;

typedef struct led_inp_s
{
  led_fct_te fct_e; // Desired LED mode

}led_inp_ts;

typedef struct led_prm_s
{
  uint16_t tiCyc_us_u16;       // Loop/cycle time [us]
  uint16_t tiBlinkFast_ms_u16; // Blink fast period [ms]
  uint16_t tiBlinkSlow_ms_u16; // Blink slow period [ms]

}led_prm_ts;

typedef struct led_prv_s
{
  uint32_t cnt_u32;          // Counter for blinking
  uint32_t limBlinkSlow_u32; // Counter limit for slow blinking
  uint32_t limBlinkFast_u32; // Counter limit for fast blinking

}led_prv_ts;

typedef struct led_outp_s
{
  uint8_t flgOn_u8;  // LED signal

}led_outp_ts;

typedef struct led_s
{
  led_inp_ts  inp_s;
  led_prm_ts  prm_s;
  led_prv_ts  prv_s;
  led_outp_ts outp_s;

}led_ts;

/****************** Public Data *****************/

/******************* Interface ******************/

LED_SCOPE uint8_t led_init( led_ts *inst_ps );
LED_SCOPE void led( led_ts *inst_ps );

#endif /* LED_H_ */
