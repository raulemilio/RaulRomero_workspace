/*
 * BMXX80.c
 *
 *	The MIT License.
 *	Based on Adafuit libraries.
 *  Created on: 10.08.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "delays.h"
#include "BMPXX80.h"

//
//	 Private variables
//
SPI_HandleTypeDef *spi_h;

uint8_t  _temperature_res, _pressure_oversampling,  _mode;
int16_t  t2, t3;
uint16_t t1;
int32_t  t_fine;

//
//	Functions
//
uint8_t BMP280_Read8(uint8_t addr)
{
    uint8_t tmp[2];
	tmp[0] = addr;
	tmp[0] |= (1<<7);//el bit mas sig debe estar en 1 para lectura
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_8,BMP280_TIMEOUT);
	return tmp[1];
}

uint16_t BMP280_Read16(uint8_t addr)
{
	uint8_t tmp[3];
	tmp[0] = addr;
	tmp[0] |= (1<<7);
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_16,BMP280_TIMEOUT);
	return ((tmp[1] << 8) | tmp[2]);
}

uint16_t BMP280_Read16LE(uint8_t addr)
{
	uint16_t tmp;
	tmp = BMP280_Read16(addr);
	return (tmp >> 8) | (tmp << 8);
}

void BMP280_Write8(uint8_t address, uint8_t data)
{
	uint8_t tmp[2];
	tmp[0] = address;
	tmp[0] &=~(1<<7);//el bit mas sig debe estar en 0 para escritura
	tmp[1] = data;
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_8,BMP280_TIMEOUT);
}

uint32_t BMP280_Read24(uint8_t addr)
{
	uint8_t tmp[4];
	tmp[0] = addr;
	tmp[0] |= (1<<7);//el bit mas sig debe estar en 1 para lectura
	BMP280_SPI_TransmitReceive(tmp,BMP280_SIZE_24,BMP280_TIMEOUT);
	return ((tmp[1] << 16) | tmp[2] << 8 | tmp[3]);
}

void BMP280_Init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t mode)
{
	spi_h = spi_handler;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	if (mode > BMP280_NORMALMODE)
	    mode = BMP280_NORMALMODE;
	_mode = mode;
	if(mode == BMP280_FORCEDMODE)
		mode = BMP280_SLEEPMODE;

	if (temperature_resolution > BMP280_TEMPERATURE_20BIT)
		temperature_resolution = BMP280_TEMPERATURE_20BIT;
	_temperature_res = temperature_resolution;

	while(BMP280_Read8(BMP280_CHIPID) != 0x58);

	/* read calibration data */
	t1 = BMP280_Read16LE(BMP280_DIG_T1);
	t2 = BMP280_Read16LE(BMP280_DIG_T2);
	t3 = BMP280_Read16LE(BMP280_DIG_T3);

	BMP280_Write8(BMP280_CONTROL, (temperature_resolution<<5)| mode);
}

float BMP280_ReadTemperature(void)
{
  int32_t var1, var2;

  if(_mode == BMP280_FORCEDMODE)
  {
	  uint8_t mode;
	  uint8_t ctrl = BMP280_Read8(BMP280_CONTROL);
	  ctrl &= ~(0x03); // si no es forceMode
	  ctrl |= BMP280_FORCEDMODE;
	  BMP280_Write8(BMP280_CONTROL, ctrl);

	  mode = BMP280_Read8(BMP280_CONTROL); 	// Read written mode
	  mode &= 0x03;							// Do not work without it...

	  if(mode == BMP280_FORCEDMODE)
	  {
		  while(1) // Wait for end of conversion
		  {
			  mode = BMP280_Read8(BMP280_CONTROL);
			  mode &= 0x03;
			  if(mode == BMP280_SLEEPMODE)
				  break;
		  }

		  int32_t adc_T = BMP280_Read24(BMP280_TEMPDATA);
		  adc_T >>= 4;

		  var1  = ((((adc_T>>3) - ((int32_t)t1 <<1))) *
				  ((int32_t)t2)) >> 11;

		  var2  = (((((adc_T>>4) - ((int32_t)t1)) *
				  ((adc_T>>4) - ((int32_t)t1))) >> 12) *
				  ((int32_t)t3)) >> 14;

		  t_fine = var1 + var2;

		  float T  = (t_fine * 5 + 128) >> 8;
		  return T/100;
	  }
  }
  return -99;
}



