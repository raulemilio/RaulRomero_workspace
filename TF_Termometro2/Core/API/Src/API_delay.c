/*
 * API_dalay.c
 *
 *  Created on: 17 ago. 2023
 *      Author: rome
 */
#include "API_delay.h"

void delayInit( delay_t * delay, tick_t duration )
  {
	  delay->duration=duration;
	  delay->running=false;
  }

bool_t delayRead( delay_t * delay )
  {
	  bool_t ret=false;

	  //if(delay==NULL)
		//  Error_Handler();

	  if(delay->running==false){
		  delay->startTime=HAL_GetTick();
		  delay->running=true;
	  }
	  else
	  if(HAL_GetTick()-delay->startTime>= delay->duration){
		  delay->running=false;
		  ret=true;
	  }
	  return ret;
  }

void delayWrite( delay_t * delay, tick_t duration )
  {
	  if(delay==NULL || duration<=0)
		  Error_Handler();
	  delay->duration=duration;
  }

