
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#define DEBOUNCE_TIME 40   // tiempo para FSM (antirebote)
// tiempos de toggle del LED2
#define BLINKY_TIME_1 100
#define BLINKY_TIME_2 500

// declaración de funciones públicas
void debounceFSM_init(void); // Se carga el estado inicial de la FSM
void debounceFSM_update(void);	// Actualización de la FSM

