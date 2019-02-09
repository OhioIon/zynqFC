#ifndef BAS_H
#define BAS_H

#ifdef BAS_C
#define BAS_SCOPE
#else
#define BAS_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdlib.h>
#include <stdint.h>
#include <sleep.h>

#include "fabric.h"
#include "gpio.h"
#include "spi.h"

/******************* Defines ********************/

/******************** Types *********************/

typedef struct Bas_s
{

}Bas_ts;

/****************** Public Data *****************/

BAS_SCOPE Bas_ts Bas_s;

/******************* Interface ******************/

BAS_SCOPE uint8_t Bas_init( void );

#endif // BAS_H
