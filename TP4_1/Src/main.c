 /** TP4. Punto1
  ******************************************************************************
  *	ESPECIALIZACIÓN EN SISTEMAS EMBEBIDOS
  *	2023
  *	PROGRAMACIÓN DE MICROPROCESADORES
  *	TRABAJO PŔACTICO 4.
  *
  *	PLACA DE DESARROLLO UTILIZADA: NUCLEO-144 (STM32F429ZI)
  *
  *	RAÚL EMILIO ROMERO
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_delay.h" // variables y funciones de delay no bloqueante
#include <stdbool.h>   // variables tipo bool

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */
/* Private typedef -----------------------------------------------------------*/

  delay_t debounceDelay; // variable de delay no bloqueante usado para tiempo antirebote
  // variable para la FSM
  typedef enum{
	   BUTTON_UP,
	   BUTTON_FALLING,
	   BUTTON_DOWN,
	   BUTTON_RAISING,
  } debounceState_t;

  debounceState_t debounce_state; // variable de la FSM

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void); // si la máquina entra en un estado no conocido
static void debounceFSM_init(void);		// Se carga el estado inicial de la FSM
static void debounceFSM_update(void);	// Actualización de la FSM
void buttonPressed(void);			// Invierte el estado del LED1
void buttonReleased(void);		// Invierte el estado del LED3

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
  // Configuración inicial
  debounceFSM_init();

  while (1)
  {
	  // esta máquina sirve para trabajar con la lectura de un botón y evitar los
	  // rebotes
	  // Actualización de la máquina
	  debounceFSM_update();
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

static void debounceFSM_init(void)
{
	  // Inicialización de la variable de estado
	  debounce_state=BUTTON_UP;
	  //GPIO_INPUT
	  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	  //GPIO_OUPTUP
	  BSP_LED_Init(LED1); // Utilizado en estado FALLING. Función buttonPressed()
	  BSP_LED_Init(LED3); // Utilizado en estado RAISING. Función buttonReleased()
	  BSP_LED_Init(LED2); // Utilizado en estado de ERROR
	  // inicialización del tiempo de delay para el retardo
	  delayInit(&debounceDelay,DEBOUNCE_TIME);
}

static void debounceFSM_update(void)
{
	  switch (debounce_state) {
	  		case BUTTON_UP:
	  			// Chequear condiciones de transición de estado
	  			if(BSP_PB_GetState(BUTTON_USER)==1){
	  				// Cambio a próximo estado
	  				debounce_state = BUTTON_FALLING;
	  			}else
	  			{
	  				// Vuelvo a estado anterior
	  				debounce_state = BUTTON_UP;
	  			}
	  		break;

	  		case BUTTON_FALLING:
	  			// Se verifica que haya pasado el tiempo de delay para el antirebote y
	  			// además que el el botón esté pulsado
	  			if((delayRead(&debounceDelay)==true)&(BSP_PB_GetState(BUTTON_USER)==1))
	  			{
	  				// cambio de estado del LED1
	  				buttonPressed();
	  				debounce_state = BUTTON_DOWN;

	  			}else
	  			{
	  				debounce_state = BUTTON_UP;
	  			}
	  		break;

	  		case BUTTON_DOWN:
	  			  // Cambio de estado
	  			  if(BSP_PB_GetState(BUTTON_USER)==0){
	  				  debounce_state = BUTTON_RAISING;
	  			  }

	  		break;

	  		case BUTTON_RAISING:
	  			// Se verifica que haya pasado el tiempo de delay para el antirebote y
	  			// además que el el botón esté soltado
	  			if((delayRead(&debounceDelay)==true)&(BSP_PB_GetState(BUTTON_USER)==0))
	  			{
	  				buttonReleased();
	  				debounce_state = BUTTON_UP;


	  			}else
	  			{
	  				debounce_state = BUTTON_DOWN;
	  			}

	  		break;

	  		default:
	  			// si no se reconoce un estado conocido se reinicia la máquina
	  			debounceFSM_init();
	  			// se enciende el LED3 y se queda en estado de error permamente
	  			Error_Handler();
	  		break;
	    }
}

void buttonPressed(void)
{
	// presionado el button se cambia el estado de LED1
	BSP_LED_Toggle(LED1);
}
void buttonReleased(void)
{
	// presionado el button se cambia el estado de LED3
	BSP_LED_Toggle(LED3);
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
