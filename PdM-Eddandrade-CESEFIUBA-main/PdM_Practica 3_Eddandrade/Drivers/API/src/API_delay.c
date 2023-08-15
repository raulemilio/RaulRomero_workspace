/*
 * API_delay.c
 *
 *  Created on: Jul 9, 2023
 *      Author: edda
 */
#include "API_delay.h"

/* delayInit debe cargar el valor de duración del retardo en la estructura, en el campo
correspondiente. No debe iniciar el conteo del retardo. Debe inicializar el flag running
en `false´. */
void delayInit( delay_t * delay, tick_t duration )
{
	assert(delay != NULL);
	assert(duration >= 0);
	delay->duration = duration;
	delay->running = false;
}

/*  delayRead debe verificar el estado del flag running.
○ false, tomar marca de tiempo y cambiar running a ‘true’
○ true, hacer la cuenta para saber si el tiempo del retardo se cumplió o no:
‘marca de tiempo actual - marca de tiempo inicial es mayor o igual a duración
del retardo’?
y devolver un valor booleano que indique si el tiempo se cumplió o no.
○
Cuando el tiempo se cumple se debe cambiar el flag running a false.*/

bool_t delayRead( delay_t * delay ){

	bool_t retValue = false;

	assert(delay !=NULL);
	if (delay->running == false){
		delay->startTime = HAL_GetTick();
		delay->running = true;
	}
	else{
		 if((HAL_GetTick()-delay->startTime) >= delay->duration){
			 delay->running = false;
			 retValue = true;
		 }
	}
	return retValue;
}

/*delayWrite permite cambiar el tiempo de duración de un delay existente*/

void delayWrite( delay_t * delay, tick_t duration ){
	assert(delay != NULL);
	assert(duration >=0);
	delay->duration=duration;
}

