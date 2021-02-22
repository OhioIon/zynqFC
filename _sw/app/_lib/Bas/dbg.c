#define DBG_C

/****************** Includes ********************/

#include "dbg.h"

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

#ifdef DEBUG
  void dbgRet(uint8_t retVal_u8)
  {
    if( retVal_u8 == 0 )
    {
      dbg( "ok\n" );
    }
    else
    {
      dbg( "fail\n" );
    }
  }
#endif

// EOF
