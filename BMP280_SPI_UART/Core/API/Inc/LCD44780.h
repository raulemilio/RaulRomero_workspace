/*
 * LCD44780.h
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */

#ifndef API_INC_LCD44780_H_
#define API_INC_LCD44780_H_

#include "main.h"
#include "port.h"

#define RS_BIT 0 // Register select bit
#define BL_BIT 3 // Backlight bit
#define D4_BIT 4 // Data 4 bit
#define D5_BIT 5 // Data 5 bit
#define D6_BIT 6 // Data 6 bit
#define D7_BIT 7 // Data 7 bit

#define LCD_ROWS 2 // Number of rows on the LCD
#define LCD_COLS 16 // Number of columns on the LCD


//void lcd_write_nibble(uint8_t nibble, uint8_t rs);
//void lcd_send_cmd(uint8_t cmd);
//void lcd_send_data(uint8_t data);
//void lcd_init();
//void lcd_write_string(char *str);
//void lcd_set_cursor(uint8_t row, uint8_t column);
//void lcd_clear(void);
//void lcd_backlight(uint8_t state);

void LCD44780_TransmitReceive(uint8_t nibble, uint8_t rs);
void LCD44780_SendCmd(uint8_t cmd);
void LCD44780_SendData(uint8_t data);
void LCD44780_Init();
void LCD44780_WriteString(char *str);
void LCD44780_SetCursor(uint8_t row, uint8_t column);
void LCD44780_Clear(void);
void LCD44780_Backlight(uint8_t state);

void LCD44780_UpdateTemperature(float temperature);
void LCD44780_TemperatureAlarm();
void LCD44780_SensorTemperatureError();



#endif /* API_INC_LCD44780_H_ */