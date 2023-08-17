/*
 * port.c
 *
 *  Created on: Aug 13, 2023
 *      Author: rome
 */
#include "port.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


// Variables Privadas
static SPI_HandleTypeDef hspi;
static I2C_HandleTypeDef hi2c;
static UART_HandleTypeDef huart;

// Implementaciones

/*
 * Para cada periférico configurar en la función correspondiente
 * los parámetros indicados
 */

//Funciones Públicas

/*****************************************************************************************/
void setSPI_Port_Name(SPI_HandleTypeDef hspiX)
{
	hspi=hspiX;
}
void setI2C_Port_Name(I2C_HandleTypeDef hi2cX)
{
	hi2c=hi2cX;
}
void setUART_Port_Name(UART_HandleTypeDef huartX)
{
	huart=huartX;
}
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, SPI_ENABLE_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD14 */
  GPIO_InitStruct.Pin = SPI_ENABLE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/*****************************************************************************************/

void MX_SPI_Init(void)
{
  hspi.Instance = SPI1;
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi.Init.NSS = SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 500 KHz
  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi) != HAL_OK)
  {
    Error_Handler();
  }
}
void BMP280_SPI_TransmitReceive(uint8_t buffer[2],uint8_t size,uint8_t timeOut)
{
	HAL_GPIO_WritePin(GPIOD, SPI_ENABLE_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi, buffer, buffer, size, timeOut);
	HAL_GPIO_WritePin(GPIOD, SPI_ENABLE_PIN, GPIO_PIN_SET);
}
/*****************************************************************************************/

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C_Init(void)
{
  hi2c.Instance = I2C2;
  hi2c.Init.ClockSpeed = 100000;
  hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c.Init.OwnAddress1 = 0;
  hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c.Init.OwnAddress2 = 0;
  hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

void LCDHD44780_I2C_PCF8574T_TransmitReceive(uint8_t data,uint8_t size,uint8_t timeOut)
{
	data |= 1 << EN_BIT; // protocolo i2c
	HAL_I2C_Master_Transmit(&hi2c, I2C_ADDR << 1, &data, size, timeOut);
	HAL_Delay(1);
	data &= ~(1 << EN_BIT);// protocolo i2c
	HAL_I2C_Master_Transmit(&hi2c, I2C_ADDR << 1, &data, size, timeOut);
}
/*****************************************************************************************/

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART_UART_Init(void)
{
  huart.Instance = USART3;
  huart.Init.BaudRate = 9600;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_ODD;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
  }
}
void UART_Send_Temperature(float temperature)
{
	  printf("Temperatura=%.2f\n",temperature);
}
void UART_TemperatureAlarm(void)
{
	  printf("Alarma: Err1\n");
}
void UART_SensorTemperatureError(void)
{
	  printf("Error en lectura del sensor: Err2\n");
}
void UART_SensorConnectError(void)
{
	  printf("Error en la Conexion con el sensor: Err3\n");
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/*****************************************************************************************/


