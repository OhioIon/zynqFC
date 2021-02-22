#ifndef DBG_H
#define DBG_H

#ifdef DBG_C
#define DBG_SCOPE
#else
#define DBG_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/******************* Defines ********************/

/******************** Types *********************/

/****************** Public Data *****************/

/******************* Interface ******************/

#ifdef DEBUG
  // Provide debug output
  #define dbg(fmt, ... ) printf(fmt, ##__VA_ARGS__ )
  DBG_SCOPE void dbgRet(uint8_t retVal_u8);
#else
  // Define as empty
  #define dbg(fmt, ... )
  #define dbgRet(retVal_u8)
#endif

#endif // DBG_H
