/*
 * termometroConfig.h
 *
 *  Created on: 14 ago. 2023
 *      Author: rome
 */

#ifndef INC_TERMOMETROCONFIG_H_
#define INC_TERMOMETROCONFIG_H_

/**********************************
 * Parámetros generales de configuración
 */
#define TEMPERATURE_MIN   0  // Temperatura mínima posible del ambiente a medir
#define TEMPERATURE_MAX   50 // Temperatura máxima posible del ambiente a medir
#define TEMPERATURE_ALARM 28 // Temperatura de alarma para generar un alerta

/**********************************
 * Periféricos a utilizar en STM32F429ZI
 */
static SPI_HandleTypeDef  hspi1;
static I2C_HandleTypeDef  hi2c2;
static UART_HandleTypeDef huart3;
/***********************************/

#endif /* INC_TERMOMETROCONFIG_H_ */