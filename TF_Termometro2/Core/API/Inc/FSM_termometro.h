/*
 * FSM_termometro.h
 *
 *  Created on: 14 ago. 2023
 *      Author: rome
 */
#ifndef SRC_FSM_TERMOMETRO_H_
#define SRC_FSM_TERMOMETRO_H_

#include "main.h"
#include "termometroConfig.h"

#include "BMP280.h"
#include "LCD44780.h"
#include "port.h"

#include <stdio.h>
#include <string.h>

/**************************************
 * Funciones de la máquina de estados principal de la API
 *
 **************************************/
// Funciones Públicas
void FSM_Termometro_Update(void);

#endif /* SRC_FSM_TERMOMETRO_H_ */
