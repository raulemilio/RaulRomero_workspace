/*
 * API_debounce.c
 *
 *  Created on: Jul 13, 2023
 *      Author: edda
 */
#include "API_debounce.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "main.h"



#define TRUE 1
#define FALSE 0
#define DEBOUNCING_TIME 40

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

static debounceState_t  Current_State;

static bool_t RISING_EDGE=FALSE, FALLING_EDGE=FALSE; // VARIABLES PARA DETECTAR POR FUERA DEL MÓDULO SI HUBO UN FLANCO ASCENDENTE O NO;


//debounceFSM_init() carga el estado inicial como  BUTTON_UP
void debounceFSM_init()
{
	/*if ( HAL_GPIO_ReadPin(Pulsador_GPIO_Port,Pulsador_Pin)!=TRUE )
	{
		Error_Handler();
	}*/

	Current_State=BUTTON_UP;
}

/* USAR ESTA PARTE DEL CODIGO PARA INDEPENDIZAR DEL MAIN CUANDO QUIERAS REESCALEAR O REUTILIZAR ESTE PROGRAMA
void buttonPressed()
{
	HAL_GPIO_TogglePin(LEDVERDE_GPIO_Port,LEDVERDE_Pin);
	}

void buttonReleased()
{
	HAL_GPIO_TogglePin(LEDROJO_GPIO_Port,LEDROJO_Pin);

	}
*/


void debounceFSM_update()	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas
{
	//Creación de estructura para el debouncing time (delay no bloqueante)
	delay_t Debouncing_Time;
	//Lectura del estado del Pulsador
	bool_t Pin_Status;
	//Vuelvo a 0 los flancos
	RISING_EDGE=0;
	FALLING_EDGE=0;

	switch (Current_State)
	{
	    case BUTTON_UP:
	    	Pin_Status = HAL_GPIO_ReadPin(Pulsador_GPIO_Port,Pulsador_Pin);
			if (Pin_Status == FALSE)
			{
				Current_State=BUTTON_FALLING;
				//Inicialización del delay
				delayInit(&Debouncing_Time, DEBOUNCING_TIME);
			}

			break;

		case BUTTON_FALLING:
			if (delayRead(&Debouncing_Time)) //Se cumplió el tiempo de debouncing
			{
				Pin_Status = HAL_GPIO_ReadPin(Pulsador_GPIO_Port,Pulsador_Pin);
				if (Pin_Status == FALSE)
					{
					FALLING_EDGE=TRUE;

					buttonPressed();

					Current_State=BUTTON_DOWN;
					}

				else Current_State=BUTTON_UP; //FALSO NEGATIVO
			}
			break;

		case BUTTON_DOWN:
			Pin_Status = HAL_GPIO_ReadPin(Pulsador_GPIO_Port,Pulsador_Pin);
			if (Pin_Status == TRUE)
			{
				Current_State=BUTTON_RAISING;
				//Inicialización del delay
				delayInit(&Debouncing_Time, DEBOUNCING_TIME);
			}
			break;

		case BUTTON_RAISING:
			if (delayRead(&Debouncing_Time)) //Se cumplió el tiempo de debouncing
						{
							Pin_Status = HAL_GPIO_ReadPin(Pulsador_GPIO_Port,Pulsador_Pin);
							if (Pin_Status == TRUE)
								{
								RISING_EDGE=TRUE;

								buttonReleased();

								Current_State=BUTTON_UP;
								}

							else Current_State=BUTTON_DOWN; //FALSO POSITIVO
						}
			break;

	}

}


bool_t readKey()
{
	bool_t aux=FALLING_EDGE;
	FALLING_EDGE=FALSE;
	return aux;
}
/*esta funcion hace lo mismo que readKey es solo por completitud de nomenclatura*/
bool_t readFallingEdge()
{
	bool_t aux=FALLING_EDGE;
	FALLING_EDGE=FALSE;
	return aux;
}

bool_t readRaisingEdge()
{
	bool_t aux=RISING_EDGE;
	RISING_EDGE=FALSE;
	return aux;
}
