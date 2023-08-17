/*
 * LCD44780.c
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */
#include "LCD44780.h"

//Variables Privadas
static uint8_t backlight_state = BLACKLIGHT_ON; // Encender ilumninación

//Funciones Privadas
static void LCD44780_TransmitReceive(uint8_t nibble, uint8_t rs);
static void LCD44780_SendCmd(uint8_t cmd);
static void LCD44780_SendData(uint8_t data);

static void LCD44780_WriteString(char *str);
static void LCD44780_SetCursor(uint8_t row, uint8_t column);
static void LCD44780_Clear(void);
static void LCD44780_Backlight(uint8_t state);

//Implementaciones

/*FUNCIONES DE COMUNICACIÓN*************************************************/
static void LCD44780_TransmitReceive(uint8_t nibble, uint8_t rs) {
  uint8_t data = nibble << D4_BIT;
  data |= rs << RS_BIT;
  data |= backlight_state << BL_BIT;
  //Comunicación por I2C
  LCDHD44780_I2C_PCF8574T_TransmitReceive(data,I2C_SIZE,I2C_TIMEOUT);

}

static void LCD44780_SendCmd(uint8_t cmd) {
  uint8_t upper_nibble = cmd >> 4;
  uint8_t lower_nibble = cmd & 0x0F;
  LCD44780_TransmitReceive(upper_nibble, MODE_CMD);
  LCD44780_TransmitReceive(lower_nibble, MODE_CMD);
}

static void LCD44780_SendData(uint8_t data) {
  uint8_t upper_nibble = data >> 4;
  uint8_t lower_nibble = data & 0x0F;
  LCD44780_TransmitReceive(upper_nibble, MODE_DATA);
  LCD44780_TransmitReceive(lower_nibble, MODE_DATA);
}

static void LCD44780_WriteString(char *str) {
  while (*str) {
	  LCD44780_SendData(*str++);
  }
}

static void LCD44780_SetCursor(uint8_t row, uint8_t column) {
    uint8_t address;
    switch (row) {
        case 0:
            address = 0x00;
            break;
        case 1:
            address = 0x40;
            break;
        default:
            address = 0x00;
    }
    address += column;
    LCD44780_SendCmd(0x80 | address);
}

static void LCD44780_Clear(void) {
	LCD44780_SendCmd(CMD_INIT_0);
}
// Encendido de la iluminación
static void LCD44780_Backlight(uint8_t state) {
  if (state) {
    backlight_state = BLACKLIGHT_ON;
  } else {
    backlight_state = BLACKLIGHT_OFF;
  }
}
/******************************************************************************/

//Publicas
/*FUNCIONES DE USUARIO*************************************************/
void LCD44780_Init() {
  char *labelTemperature = "Temperatura:";
  LCD44780_TransmitReceive(CMD_INIT_1, MODE_CMD);
  LCD44780_TransmitReceive(CMD_INIT_1, MODE_CMD);
  LCD44780_TransmitReceive(CMD_INIT_1, MODE_CMD);
  LCD44780_TransmitReceive(CMD_INIT_2, MODE_CMD);

  LCD44780_SendCmd(0x28);
  LCD44780_SendCmd(0x0C);
  LCD44780_SendCmd(0x06);
  LCD44780_SendCmd(0x01);

  LCD44780_Backlight(BLACKLIGHT_ON);
}
void LCD44780_UpdateTemperature(float temperature)
{
		  char *labelTemperature = "Temperatura:";
		  char *labelTemperatureUnit = "C";
		  char temperatureString[10];

		  sprintf(temperatureString, "%.2f", temperature);
		  LCD44780_Clear();
		  LCD44780_SetCursor(0, 0);
		  LCD44780_WriteString(labelTemperature);
		  LCD44780_SetCursor(1, 0);
		  LCD44780_WriteString(temperatureString);
		  memset(temperatureString, 0, sizeof(temperatureString));
		  LCD44780_SetCursor(1, 6);
		  LCD44780_WriteString(labelTemperatureUnit );
		  HAL_Delay(TEMPERATURE_TIME_UPDATE);
}

void LCD44780_TemperatureAlarm()
{
	  char *labelTemperatureAlarm = "Alarma";
	  LCD44780_SetCursor(1, 9);
	  LCD44780_WriteString(labelTemperatureAlarm );
	  HAL_Delay(TEMPERATURE_TIME_UPDATE);
}
void LCD44780_SensorTemperatureError()
{
	  char *labelSensorTemperatureError = "Error de sensor";
	  LCD44780_Clear();
	  LCD44780_SetCursor(1, 0);
	  LCD44780_WriteString(labelSensorTemperatureError );
	  HAL_Delay(TEMPERATURE_TIME_UPDATE);
}
void LCD44780_SensorConnectError()
{
	  char *labelSensorConnectError = "Error de Conn";
	  LCD44780_Clear();
	  LCD44780_SetCursor(1, 0);
	  LCD44780_WriteString(labelSensorConnectError );
	  HAL_Delay(TEMPERATURE_TIME_UPDATE);
}
/******************************************************************************/
