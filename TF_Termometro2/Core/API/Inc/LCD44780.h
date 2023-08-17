/*
 * LCD44780.h
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */

#ifndef API_INC_LCD44780_H_
#define API_INC_LCD44780_H_

#include "main.h"
#include "termometroConfig.h"
#include "port.h"

#define RS_BIT 0 // Register select bit
#define BL_BIT 3 // Backlight bit
#define D4_BIT 4 // Data 4 bit
#define D5_BIT 5 // Data 5 bit
#define D6_BIT 6 // Data 6 bit
#define D7_BIT 7 // Data 7 bit

#define LCD_ROWS 2 // Número de filas LCD
#define LCD_COLS 16 // Número de columnas LCD

// Enceder/apagar iluminación LCD
#define BLACKLIGHT_ON  1
#define BLACKLIGHT_OFF 0
// modo de escritura Comando/data
#define MODE_CMD		0
#define MODE_DATA		1
// Comandos de inicialización
#define CMD_INIT_0	   0x01
#define CMD_INIT_2	   0x02
#define CMD_INIT_1	   0x03

//Funciones Públicas
void LCD44780_Init(void);
void LCD44780_UpdateTemperature(float temperature);
void LCD44780_TemperatureAlarm(void);
void LCD44780_SensorTemperatureError(void);
void LCD44780_SensorConnectError(void);

#endif /* API_INC_LCD44780_H_ */
