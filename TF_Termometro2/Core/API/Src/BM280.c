/*
 * BMP280.h
 *
 *  Created on: 14 ago. 2023
 *      Author: rome
 */
#include "BMP280.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "API_delay.h"

//Variables Privadas
static SPI_HandleTypeDef *spi_h;
static uint8_t  _temperature_res,_mode;
static int16_t  t2, t3;
static uint16_t t1;
static int32_t  t_fine;
static delay_t delayTimeConvertion; // Delay para la espera de la lectura del adc

// Funciones Privadas
static uint8_t BMP280_Read8(uint8_t addr);
static uint16_t BMP280_Read16(uint8_t addr);
static uint16_t BMP280_Read16LE(uint8_t addr);
static uint32_t BMP280_Read24(uint8_t addr);
static void BMP280_Write8(uint8_t address, uint8_t data);

// Implementaciones

//Privadas
/*************************************************************************************/
// Los datos contenidos en los registros tienen diferentes tamaños
// por este motivo es necesario diferentes funciones que tengan en cuenta cada tamaño
static uint8_t BMP280_Read8(uint8_t addr)
{
    uint8_t tmp[2];
	tmp[0] = addr;
	tmp[0] |= (1<<BMP280_MODE_BIT_RW); // READ 0
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_8,BMP280_TIMEOUT);
	return tmp[1];
}

static uint16_t BMP280_Read16(uint8_t addr)
{
	uint8_t tmp[3];
	tmp[0] = addr;
	tmp[0] |= (1<<BMP280_MODE_BIT_RW); // READ 0
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_16,BMP280_TIMEOUT);
	return ((tmp[1] << 8) | tmp[2]);
}

static uint16_t BMP280_Read16LE(uint8_t addr)
{
	uint16_t tmp;
	tmp = BMP280_Read16(addr);
	return (tmp >> 8) | (tmp << 8);
}

static void BMP280_Write8(uint8_t address, uint8_t data)
{
	uint8_t tmp[2];
	tmp[0] = address;
	tmp[0] &=~(1<<BMP280_MODE_BIT_RW); // WRITE 1
	tmp[1] = data;
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_8,BMP280_TIMEOUT);
}

static uint32_t BMP280_Read24(uint8_t addr)
{
	uint8_t tmp[4];
	tmp[0] = addr;
	tmp[0] |= (1<<BMP280_MODE_BIT_RW); // READ 0
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_24,BMP280_TIMEOUT);
	return ((tmp[1] << 16) | tmp[2] << 8 | tmp[3]);
}
/*************************************************************************************/
//Públicas
uint8_t BMP280_Init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t mode)
{
	spi_h = spi_handler;
	uint8_t ret;
	uint8_t id;

	if (mode > BMP280_NORMALMODE)
	    mode = BMP280_NORMALMODE;
	_mode = mode;
	if(mode == BMP280_FORCEDMODE)
		mode = BMP280_SLEEPMODE;

	if (temperature_resolution > BMP280_TEMPERATURE_20BIT)
		temperature_resolution = BMP280_TEMPERATURE_20BIT;
	_temperature_res = temperature_resolution;

	for(uint8_t i=0;i<COUNT_MAX_CONNECT;i++)
	{
		id=BMP280_Read8(BMP280_CHIPID);
		if (id==BMP280_DEVICE_ID){
			ret=0;// sensor detectado
			break;
		}else
		{
			ret=-1;
		}
	}
	//Calibración
	t1 = BMP280_Read16LE(BMP280_DIG_T1);
	t2 = BMP280_Read16LE(BMP280_DIG_T2);
	t3 = BMP280_Read16LE(BMP280_DIG_T3);

	BMP280_Write8(BMP280_CONTROL, (temperature_resolution<< BMP280_BIT_RESOLUTION )| mode);
	return ret;
}

float BMP280_ReadTemperature(void)
{
  float ret;  // Si la lectura pudo realizarse ret es el valor de temperatura
  	  	  	  // Si la lectura no pudo realizarse ret=-1
  int32_t var1, var2;
  float T;
  delayInit(&delayTimeConvertion,TIME_MAX_CONVERTION_FINISH);

  if(_mode == BMP280_FORCEDMODE)
  {
	  uint8_t mode;
	  uint8_t ctrl = BMP280_Read8(BMP280_CONTROL);// Se verifica el modo FORCE
	  ctrl &= ~(0x03);
	  ctrl |= BMP280_FORCEDMODE;
	  // Debido a que no puede accederse al dato si no está en FORCEMODE
	  // Se carga FORCEMODE si es que no estaba
	  BMP280_Write8(BMP280_CONTROL, ctrl);
	  mode = BMP280_Read8(BMP280_CONTROL);
	  mode &= 0x03;

	  if(mode == BMP280_FORCEDMODE)
	  {
		  /* Cuando termina la conversión el sensor vuelve al estado
		   * SLEEPMODE
		   * Solo puede leerse el dato cuando terminó la conversión,
		   * es decir en SLEEPMODE.
		   */
		  while(delayRead(&delayTimeConvertion!=CONVERTION_MAX_FINISH)){
			mode = BMP280_Read8(BMP280_CONTROL);
			mode &= 0x03;
			  if (mode == BMP280_SLEEPMODE){
			  		break;
			  	}else
			  	{
			  	ret=-1;
			  	}
		  }
		  // Ecuación de conversión dada por el fabricante
		  // Temperatura en grados Celsius
		  int32_t adc_T = BMP280_Read24(BMP280_TEMPDATA);
		  adc_T >>= 4;

		  var1  = ((((adc_T>>3) - ((int32_t)t1 <<1))) *
				  ((int32_t)t2)) >> 11;

		  var2  = (((((adc_T>>4) - ((int32_t)t1)) *
				  ((adc_T>>4) - ((int32_t)t1))) >> 12) *
				  ((int32_t)t3)) >> 14;

		  t_fine = var1 + var2;

		  T  = (t_fine * 5 + 128) >> 8;
		  T=T/100;
		  ret=T;
	  }
  }else
  {
	  ret=-1;
  }
  return ret;
}
/*************************************************************************************/


