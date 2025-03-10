 /** TP3
  ******************************************************************************
  *	ESPECIALIZACIÓN EN SISTEMAS EMBEBIDOS
  *	2023
  *	PROGRAMACIÓN DE MICROPROCESADORES
  *	TRABAJO PŔACTICO 3.RETARDOS NO BLOQUEANTES
  *	OBJETIVO:
  *	Implementar un módulo de software para trabajar con retardos no bloqueantes
  *	a partir de las funciones creadas en la práctica 2.
  *
  *	PLACA DE DESARROLLO UTILIZADA: NUCLEO-144 (STM32F429ZI)
  *
  *	RAÚL EMILIO ROMERO
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_delay.h"
#include <stdbool.h>

// El trabajo práctico pide 200 para todos los led
// pero aquí puede configurarse individualmente
#define BLINKY_TIME1 200
#define BLINKY_TIME2 200
#define BLINKY_TIME3 200

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Initialize BSP Led for LED1-LED2-LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* delay varibles */
  delay_t delayBlinky1;
  delay_t delayBlinky2;
  delay_t delayBlinky3;

  // Variables auxiliares

  // En cada ciclo de ON-OFF de cada led solo una de las variables count varia de 0 a 2
  // esta variable permite que sólo se ejecute el código correspondiente a cada led por vez
  int count1=0;
  int count2=0;
  int count3=0;

  // Las variables retX se utilizan para saber cuando se cumplió el tiempo de delay
  // Cada led se inicia en estado OFF hasta que la variable retX==true,
  // luego se cambia de estado al led (ON) y se reinicia el contador correspondiente
  // (delayInit).
  // y se incrementa la variable countX. De esta manera cada led solo puede estar una vez
  // en OFF y una vez en ON en cada iteración del while(1)
  bool_t ret1;
  bool_t ret2;
  bool_t ret3;


  delayInit(&delayBlinky1,BLINKY_TIME1);
  delayInit(&delayBlinky2,BLINKY_TIME2);
  delayInit(&delayBlinky3,BLINKY_TIME3);

  while (1)
  {
	  if(count1<2){
		  ret1=delayRead(&delayBlinky1);
		  if((ret1==true)){
			  BSP_LED_Toggle(LED1);
			  delayInit(&delayBlinky1,BLINKY_TIME1);
			  count1++;
		  }
	  }
	  // No se evaluan delayRead(&delayBlinky2) hasta que no termine un ciclo de OFF-ON LED1
	  if(count1==2){
		  ret2=delayRead(&delayBlinky2);
		  if((ret2==true)&(count2<2)){
			  BSP_LED_Toggle(LED2);
			  delayInit(&delayBlinky2,BLINKY_TIME2);
			  count2++;
		  }
	  }
	  // No se evaluan delayRead(&delayBlinky3) hasta que no termine un ciclo de OFF-ON LED2
	  if(count2==2){
		  ret3=delayRead(&delayBlinky3);
		  if((ret3==true)&(count3<2)){
			  BSP_LED_Toggle(LED3);
			  delayInit(&delayBlinky3,BLINKY_TIME3);
			  count3++;
		  }
	  }
	  // reinicio de variables
	  if((count1==2)&(count2==2)&(count3==2)){
		  count1=0;
		  count2=0;
		  count3=0;
		  delayInit(&delayBlinky1,BLINKY_TIME1);
		  delayInit(&delayBlinky2,BLINKY_TIME1);
		  delayInit(&delayBlinky3,BLINKY_TIME1);
	  }
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
