/*
 * FSM_termometro.c
 *
 *  Created on: 14 ago. 2023
 *      Author: rome
 */
#include "FSM_termometro.h"
#include "termometroConfig.h"

// salida de la función FSM_Termometro_Measure()
#define CONNECT_OK     0
#define CONNECT_ERROR  -1
#define MEASURE_OK     0
#define MEASURE_ERROR  -1

typedef enum{
	INIT,
	CONNECT,
	MEASURE,
	RANGE,
}fsmState_t;

// Variables Privadas
static fsmState_t fsm_state=INIT;
static int8_t errorConnect;
static int8_t errorMeasure;
static float temperature;

// Funciones Privadas
static void FSM_Termometro_Init(void);
static uint8_t FSM_Termometro_Connect(void);
static int8_t FSM_Termometro_Measure(void);
static void FSM_Termometro_Range(void);

// Implementaciones

//Privadas*****************************************************************************/
//Configuración e inicialización de puertos y periféricos
static void FSM_Termometro_Init(void)
{
	  setSPI_Port_Name(hspi1);
	  setI2C_Port_Name(hi2c2);
	  setUART_Port_Name(huart3);

	  MX_GPIO_Init();
	  MX_SPI_Init();
	  MX_USART_UART_Init();
	  MX_I2C_Init();
	  LCD44780_Init();
}
// La medición se realiza en modo Forzado (FORCEMODE) y a la frecuencia de muestreo menor
// Verificación de la conexión del sensor con el microprocesador
// ver datasheet BMP280
static uint8_t FSM_Termometro_Connect(void)
{
	 uint8_t ret;
	 ret=BMP280_Init(&hspi1, BMP280_TEMPERATURE_16BIT, BMP280_FORCEDMODE);

	 if (ret!=0){
		  ret =-1; // No se detectó el sensor
		  UART_SensorConnectError();
		  LCD44780_SensorConnectError();
	  }
	 return ret;
}

int8_t FSM_Termometro_Measure(void)
{
	  int8_t ret;
	  temperature=BMP280_ReadTemperature();

	  if ((temperature<TEMPERATURE_MAX) && (temperature>TEMPERATURE_MIN)){
		  UART_Send_Temperature(temperature);
		  LCD44780_UpdateTemperature(temperature);
		  ret =0; // Dentro del rango
	  }else
	  	  {
		  	  LCD44780_SensorTemperatureError();
		  	  UART_SensorTemperatureError();
		  	  ret=-1; // Fuera del rango
	  	  }
	  return ret;
}

static void FSM_Termometro_Range(void)
{
	if(temperature>TEMPERATURE_ALARM)
	{
		LCD44780_TemperatureAlarm();
		UART_TemperatureAlarm();
	}
}

/**************************************************************************************/
//Funciones Públicas

// La máquina está diseñada a partir de la máquina de estados propuesta en el datasheet
// del sensor
// Se adaptó la máquina para que el sistema soporte conexiones/desconexiones del sensor
void FSM_Termometro_Update(void)
{
	  switch (fsm_state) {

	  	  	  case INIT:
	  	  		    FSM_Termometro_Init();
	                fsm_state=CONNECT;
	  	  	  break;

	  	  	  case CONNECT:
	  	  		    errorConnect=FSM_Termometro_Connect();
	  	  		    if(errorConnect==CONNECT_OK){
		  	  		    fsm_state=MEASURE;
	  	  		    }else
	  	  		    	if(errorConnect==CONNECT_ERROR)
	  	  		    	{
	  	  		    	fsm_state=INIT;
	  	  		    	}
	  	  	  break;

	  	  	  case MEASURE:
	  	  		    errorMeasure=FSM_Termometro_Measure();
	  	  		    if(errorMeasure==MEASURE_OK){
		  	  		    fsm_state=RANGE;
	  	  		    }else
	  	  		    	if(errorMeasure==MEASURE_ERROR)
	  	  		    	{
	  	  		    	fsm_state=CONNECT;
	  	  		    	}
	  	  	  break;

	  	  	  case RANGE:
	  	  		    FSM_Termometro_Range();
	  	  		    fsm_state=MEASURE;
	  	  	  break;

	  	  	  default:
	  	  		fsm_state=INIT;
	  	  	  break;
	  	  }
}

