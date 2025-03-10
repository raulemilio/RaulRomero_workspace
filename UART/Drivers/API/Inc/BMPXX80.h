/*
 * BMPXX0.h
 *
 *  The MIT License.
 *  Based on Adafuit libraries.
 *  Created on: 10.08.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */

#ifndef BMPXX80_H_
#define BMPXX80_H_

//
// Temperature resolution
#define BMP280_TEMPERATURE_16BIT 1
#define BMP280_TEMPERATURE_17BIT 2
#define BMP280_TEMPERATURE_18BIT 3
#define BMP280_TEMPERATURE_19BIT 4
#define BMP280_TEMPERATURE_20BIT 5

// Pressure oversampling
#define BMP280_ULTRALOWPOWER	1
#define BMP280_LOWPOWER			2
#define BMP280_STANDARD			3
#define BMP280_HIGHRES			4
#define BMP280_ULTRAHIGHRES		5

// Mode
#define BMP280_SLEEPMODE		0
#define BMP280_FORCEDMODE		1
#define BMP280_NORMALMODE		3
//
//	Coeffs registers
//
#define	BMP280_DIG_T1		0x88
#define	BMP280_DIG_T2		0x8A
#define	BMP280_DIG_T3		0x8C
#define	BMP280_DIG_P1		0x8E
#define	BMP280_DIG_P2		0x90
#define	BMP280_DIG_P3		0x92
#define	BMP280_DIG_P4		0x94
#define	BMP280_DIG_P5		0x96
#define	BMP280_DIG_P6		0x98
#define	BMP280_DIG_P7		0x9A
#define	BMP280_DIG_P8		0x9C
#define	BMP280_DIG_P9		0x9E

//
//	Registers
//
#define	BMP280_CHIPID			0xD0
#define	BMP280_VERSION			0xD1
#define	BMP280_SOFTRESET		0xE0
#define	BMP280_CAL26			0xE1  // R calibration stored in 0xE1-0xF0
#define	BMP280_STATUS			0xF3
#define	BMP280_CONTROL			0xF4
#define	BMP280_CONFIG			0xF5
#define	BMP280_PRESSUREDATA		0xF7
#define	BMP280_TEMPDATA			0xFA

//
//	Control bits
//
#define	BMP280_MEASURING			(1<<3) // Conversion in progress

//
// User functions
//
void BMP280_Init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t pressure_oversampling, uint8_t mode);

void BMP280_SetConfig(uint8_t standby_time, uint8_t filter);

float BMP280_ReadTemperature(void);
int32_t BMP280_ReadPressure(void);
uint8_t BMP280_ReadTemperatureAndPressure(float *temperature, int32_t *pressure);

float BMP280_ReadAltitude(float sea_level_pa);

#endif /* BMPXX80_H_ */
