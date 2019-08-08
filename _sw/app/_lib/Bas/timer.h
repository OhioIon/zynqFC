#ifndef TIMER_H
#define TIMER_H

#ifdef TIMER_C
#define TIMER_SCOPE
#else
#define TIMER_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>
#include <stdbool.h>

#include <xscutimer.h>

/******************* Defines ********************/

/******************** Types *********************/

/****************** Public Data *****************/

TIMER_SCOPE bool flgTimerIntr_l; // Timer interrupt handshake
TIMER_SCOPE XScuTimer timer_s;   // SCU timer instance

/******************* Interface ******************/

TIMER_SCOPE uint8_t timer_init( void );

TIMER_SCOPE void timer_start( void );
TIMER_SCOPE void timer_stop( void );

TIMER_SCOPE void timer_IntrHandler( void *p );

#endif // TIMER_H
