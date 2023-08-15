/*
 * API_debounce.h
 *
 *  Created on: Jul 13, 2023
 *      Author: edda
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void debounceFSM_init();
void debounceFSM_update();
/* La función readKey debe leer una variable interna del módulo y devolver true o false si la tecla fue presionada.  Si devuelve true, debe resetear (poner en false) el estado de la variable.*/
bool readKey();


#endif /* API_INC_API_DEBOUNCE_H_ */
