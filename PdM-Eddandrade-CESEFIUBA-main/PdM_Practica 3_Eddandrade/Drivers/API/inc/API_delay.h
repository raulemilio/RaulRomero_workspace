/*
 * API_delay.h
 *
 *  Created on: Jul 9, 2023
 *      Author: edda
 */

#ifndef API_API_DELAY_H_
#define API_API_DELAY_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile? ->stdint

typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile? ->stdbool

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

void delayInit( delay_t * delay, tick_t duration );

bool_t delayRead( delay_t * delay );

void delayWrite( delay_t * delay, tick_t duration );


#endif /* API_API_DELAY_H_ */
