#ifndef INTR_H_
#define INTR_H_

#ifdef INTR_C
#define INTR_SCOPE
#else
#define INTR_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

/****************** Public Data *****************/

/******************* Interface ******************/

INTR_SCOPE uint8_t intr_init( void );

INTR_SCOPE void intr_enable( void );
INTR_SCOPE void intr_disable( void );

#endif /* INTR_H_ */
