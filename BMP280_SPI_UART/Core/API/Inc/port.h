/*
 * port.h
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */

#ifndef INC_PORT_H_
#define INC_PORT_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define I2C_ADDR   0x27 // I2C address of the PCF8574
#define I2C_SIZE   1
#define I2C_TIMEOUT 100
#define EN_BIT 2 // Enable bit

#define SPI_ENABLE_PIN 	GPIO_PIN_14

extern SPI_HandleTypeDef  hspi1;
extern I2C_HandleTypeDef  hi2c2;
extern UART_HandleTypeDef huart3;

void MX_GPIO_Init(void);
void MX_SPI1_Init(void);

void MX_I2C2_Init(void);
void LCDHD44780_I2C_PCF8574T_TransmitReceive(uint8_t data,uint8_t size,uint8_t timeOut);
void BMP280_SPI_TransmitReceive(uint8_t buffer[2],uint8_t size,uint8_t timeOut);

void MX_USART3_UART_Init(void);
void UART_Send_Temperature(float temperature);
void UART_TemperatureAlarm(void);
void UART_SensorTemperatureError(void);

#endif /* INC_PORT_H_ */