/*
 * LCD44780.c
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */
#include "LCD44780.h"

uint8_t backlight_state = 1;

void LCD44780_TransmitReceive(uint8_t nibble, uint8_t rs) {
  uint8_t data = nibble << D4_BIT;
  data |= rs << RS_BIT;
  data |= backlight_state << BL_BIT; // Include backlight state in data
  LCDHD44780_I2C_PCF8574T_TransmitReceive(data,I2C_SIZE,I2C_TIMEOUT);
}

void LCD44780_SendCmd(uint8_t cmd) {
  uint8_t upper_nibble = cmd >> 4;
  uint8_t lower_nibble = cmd & 0x0F;
  LCD44780_TransmitReceive(upper_nibble, 0);
  LCD44780_TransmitReceive(lower_nibble, 0);
  if (cmd == 0x01 || cmd == 0x02) {
    HAL_Delay(2);
  }
}

void LCD44780_SendData(uint8_t data) {
  uint8_t upper_nibble = data >> 4;
  uint8_t lower_nibble = data & 0x0F;
  LCD44780_TransmitReceive(upper_nibble, 1);
  LCD44780_TransmitReceive(lower_nibble, 1);
}

void LCD44780_Init() {
  HAL_Delay(50);
  LCD44780_TransmitReceive(0x03, 0);
  HAL_Delay(5);
  LCD44780_TransmitReceive(0x03, 0);
  HAL_Delay(1);
  LCD44780_TransmitReceive(0x03, 0);
  HAL_Delay(1);
  LCD44780_TransmitReceive(0x02, 0);
  LCD44780_SendCmd(0x28);
  LCD44780_SendCmd(0x0C);
  LCD44780_SendCmd(0x06);
  LCD44780_SendCmd(0x01);
  HAL_Delay(2);
}

void LCD44780_WriteString(char *str) {
  while (*str) {
	  LCD44780_SendData(*str++);
  }
}

void LCD44780_SetCursor(uint8_t row, uint8_t column) {
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

void LCD44780_Clear(void) {
	LCD44780_SendCmd(0x01);
    HAL_Delay(2);
}

void LCD44780_Backlight(uint8_t state) {
  if (state) {
    backlight_state = 1;
  } else {
    backlight_state = 0;
  }
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
		  HAL_Delay(1000);
}

void LCD44780_TemperatureAlarm()
{
	  char *labelTemperatureAlarm = "Alarma";
	  LCD44780_SetCursor(1, 9);
	  LCD44780_WriteString(labelTemperatureAlarm );
	  HAL_Delay(500);
}
void LCD44780_SensorTemperatureError()
{
	  char *labelSensorTemperatureError = "Error de sensor";
	  LCD44780_Clear();
	  LCD44780_SetCursor(1, 0);
	  LCD44780_WriteString(labelSensorTemperatureError );
	  HAL_Delay(500);
}
//void lcd_write_nibble(uint8_t nibble, uint8_t rs) {
//  uint8_t data = nibble << D4_BIT;
//  data |= rs << RS_BIT;
//  data |= backlight_state << BL_BIT; // Include backlight state in data
//  LCDHD44780_I2C_PCF8574T_TransmitReceive(data,I2C_SIZE,I2C_TIMEOUT);
//}
//
//void lcd_send_cmd(uint8_t cmd) {
//  uint8_t upper_nibble = cmd >> 4;
//  uint8_t lower_nibble = cmd & 0x0F;
//  lcd_write_nibble(upper_nibble, 0);
//  lcd_write_nibble(lower_nibble, 0);
//  if (cmd == 0x01 || cmd == 0x02) {
//    HAL_Delay(2);
//  }
//}
//
//void lcd_send_data(uint8_t data) {
//  uint8_t upper_nibble = data >> 4;
//  uint8_t lower_nibble = data & 0x0F;
//  lcd_write_nibble(upper_nibble, 1);
//  lcd_write_nibble(lower_nibble, 1);
//}
//
//void lcd_init() {
//  HAL_Delay(50);
//  lcd_write_nibble(0x03, 0);
//  HAL_Delay(5);
//  lcd_write_nibble(0x03, 0);
//  HAL_Delay(1);
//  lcd_write_nibble(0x03, 0);
//  HAL_Delay(1);
//  lcd_write_nibble(0x02, 0);
//  lcd_send_cmd(0x28);
//  lcd_send_cmd(0x0C);
//  lcd_send_cmd(0x06);
//  lcd_send_cmd(0x01);
//  HAL_Delay(2);
//}
//
//void lcd_write_string(char *str) {
//  while (*str) {
//    lcd_send_data(*str++);
//  }
//}
//
//void lcd_set_cursor(uint8_t row, uint8_t column) {
//    uint8_t address;
//    switch (row) {
//        case 0:
//            address = 0x00;
//            break;
//        case 1:
//            address = 0x40;
//            break;
//        default:
//            address = 0x00;
//    }
//    address += column;
//    lcd_send_cmd(0x80 | address);
//}
//
//void lcd_clear(void) {
//	lcd_send_cmd(0x01);
//    HAL_Delay(2);
//}
//
//void lcd_backlight(uint8_t state) {
//  if (state) {
//    backlight_state = 1;
//  } else {
//    backlight_state = 0;
//  }
//}
//