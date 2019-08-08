#ifndef MAIN_H
#define MAIN_H

#ifdef MAIN_C
#define MAIN_SCOPE
#else
#define MAIN_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

#include <xtime_l.h>

/******************* Defines ********************/

#define TASK_TIME_US_D 125 // 125 µs task cycle time (8 kHz)

/******************** Types *********************/

typedef struct main_prv_s
{
  // Counter
  uint32_t nrCallApp_u32;        // Nr. of executions of app
  uint32_t nrRtDeadlineMiss_u32; // Nr. of missed realt-time deadlines of application task

  // Helper for runtime measurement
  XTime    tiPre_ticks_u64;      // CPU ticks prior calling app
  XTime    tiPost_ticks_u64;     // CPU ticks after calling app

}main_prv_ts;

typedef struct main_outp_s
{
  //  Runtime of app
  uint32_t tiAppRuntime_us_u32;    // Last measured [µs]
  uint32_t tiAppRuntimeMax_us_u32; // Max. measured [µs]

}main_outp_ts;

typedef struct main_s
{
  main_prv_ts  prv_s;
  main_outp_ts outp_s;

}main_ts;

/****************** Public Data *****************/

MAIN_SCOPE main_ts main_s;

/******************* Interface ******************/

MAIN_SCOPE int main( void ); // Init task
MAIN_SCOPE void app( void ); // Cyclic task

#endif // MAIN_H
