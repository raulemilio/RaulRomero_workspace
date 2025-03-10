#include "API_uart.h"


#define TIMEOUT 500 // en ms
#define UART_BAUDRATE 9600

/* UART handler declaration */
static UART_HandleTypeDef UartHandle;


// Mensajes de inicialización
static uint8_t msgInit[]="\n UART INIT \n";
static uint8_t msgInitBaudRate[]="Baud Rate:\n";
static char charBaudRate[20];

static uint16_t sizemsgInit=sizeof(msgInit);
static uint16_t sizemsgBaudRate=sizeof(msgInitBaudRate);

static void Error_Handler(void);

/*##-1- Configure the UART peripheral ######################################*/
bool_t uartInit(void)
{
	bool_t ret=false;

	  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	  /* UART configured as follows:
	      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
		                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	      - Stop Bit    = One Stop bit
	      - Parity      = ODD parity
	      - BaudRate    = 9600 baud
	      - Hardware flow control disabled (RTS and CTS signals) */
	  UartHandle.Instance        = USARTx;

	  UartHandle.Init.BaudRate   = UART_BAUDRATE;
	  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle.Init.Parity     = UART_PARITY_ODD;
	  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&UartHandle) != HAL_OK)
	  {
	    /* Initialization Error */
		ret=true;
	    Error_Handler();
	  }

	  // se indica que el puerto inició y se muestran dos parámetros de configuración

	  HAL_UART_Transmit(&UartHandle, msgInit, sizemsgInit, TIMEOUT);
	  HAL_UART_Transmit(&UartHandle, msgInitBaudRate, sizemsgBaudRate, TIMEOUT);
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)charBaudRate, sprintf(charBaudRate, "%d", UART_BAUDRATE), TIMEOUT);
}

// se envia un byte por el puerto serie
void uartSendString(uint8_t * pstring)
{
  HAL_UART_Transmit(&UartHandle, pstring, sizeof(pstring), TIMEOUT);
}
// se envian n byte por el puerto serie
void uartSendStringSize(uint8_t * pstring, uint16_t size)
{
	  HAL_UART_Transmit(&UartHandle, pstring, size, TIMEOUT);
}
// se reciben n byte por el puerto serie
void uartReceiveStringSize(uint8_t * pstringReceive, uint16_t size)
{
	HAL_UART_Receive(&UartHandle, pstringReceive, size, TIMEOUT);

}

static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}
