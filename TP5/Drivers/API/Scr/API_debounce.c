#include "API_debounce.h"

// Variables internas
  // Variable para los estados de la FSM
  typedef enum{
	   BUTTON_UP,
	   BUTTON_FALLING,
	   BUTTON_DOWN,
	   BUTTON_RAISING,
	} debounceState_t;

  static debounceState_t debounce_state; // estado de la FSM  privada estatica

  // variable interna que cambia cuando se presiona el botón
  static bool_t buttonPress; // variable interna pedida en el trabajo práctico
  // Variable interna auxiliar para hacer el toggle entre los dos tiempos 100ms y 500 ms
  static bool_t toggleBlinkyTimeLed;

  // variable interna pedida en el trabajo práctico que se actualiza cuando se
  // hace un llamado a la función readKey() y se está en el estado BUTTON_FALLING
  static bool_t global=false;

  // delay para la FSM
  static delay_t debounceDelay;
  // delay para el tiempo de toggle del LED2
  static delay_t debounceDelayBlinkyLed;

  // función pedida en el trabajo práctico que se actualiza cuando se presiona el Button
  static bool_t readKey(void);
  static void buttonPressed(void);
  static void buttonReleased(void);

  // mensajes a enviar por uart
  uint8_t sendBufferDataFlancoDes[]="Se ha producido un flanco descendente\n";
  uint8_t sendBufferDataFlancoAsc[]="Se ha producido un flanco ascendente\n";

  uint16_t sizeSendDataAsc=sizeof(sendBufferDataFlancoAsc);
  uint16_t sizeSendDataDes=sizeof(sendBufferDataFlancoDes);

void debounceFSM_init(void)
{
	  // Inicialización de la variable de estado
	  debounce_state=BUTTON_UP;
	  //GPIO_INPUT
	  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	  //GPIO_OUPTUP
	  BSP_LED_Init(LED1); // Utilizado en estado FALLING. Función buttonPressed()
	  BSP_LED_Init(LED2); // Utilizado en estado RAISING. Función buttonReleased()
	  BSP_LED_Init(LED3); // Utilizado en estado de ERROR
	  // inicialización del tiempo de delay para el retardo antirebote
	  delayInit(&debounceDelay,DEBOUNCE_TIME);
	  // inicialización del tiempo de delay para el retardo de LED2
	  delayInit(&debounceDelayBlinkyLed,BLINKY_TIME_1);
}

void debounceFSM_update(void)
{
	//Este bloque lo implemente aquí y no en main.c porque interpreté que
	// el TP pedia trabajar todo con variables internas, pero al ver la planilla
	// de corrección de pares me surgió la duda
	/******************************************************************/
	  buttonPress=readKey(); // se verifica si se presionó el button
	  // según el estado de la variable auxiliar toggleBlinkyTimeLed se
	  // carga el tiempo de toggle para el LED2
	  if(buttonPress==false){
		  if(toggleBlinkyTimeLed==true)
		  {
			  delayWrite(&debounceDelayBlinkyLed,BLINKY_TIME_1);
		  }else
		  {
			  delayWrite(&debounceDelayBlinkyLed,BLINKY_TIME_2);
		  }
		  toggleBlinkyTimeLed=!toggleBlinkyTimeLed;
	  }
	  // cambia de estado (toggle) el LED2 despúes de que la función delayRead()
	  // da como salida true
	  if(delayRead(&debounceDelayBlinkyLed)==true)
	  {
			BSP_LED_Toggle(LED2);
	  }
	  // esta máquina de estados sirve para manejar los estados del button
	  // verificamos los estados posibles
	  /*******************************************************************/

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
	  				global=true; // se pide en el TP que en este estado esta variable pase a true
	  				buttonPressed(); // estado de flanco
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
	  				buttonReleased();// estado de flanco
	  				debounce_state = BUTTON_UP;

	  			}else
	  			{
	  				debounce_state = BUTTON_DOWN;
	  			}

	  		break;

	  		default:
	  			// si no se reconoce un estado conocido se reinicia la máquina
	  			// y se enciende el LED3
	  			debounceFSM_init();
	  			BSP_LED_On(LED3);
	  		break;
	    }
}

static bool_t readKey( void )
{
	bool_t ret; // variable de retorno
	global=false; // se pide en el TP que pase a false
	// se el estado es Down ret es false
	if(debounce_state == BUTTON_DOWN){
		ret=false;
	}else
	{
		ret=true;
	}
	return ret;
}
// state button press
static void buttonPressed(void)
{
	// Se comunica el estado del flanco
	uartSendStringSize(sendBufferDataFlancoDes,sizeSendDataDes);

}
// state button release
static void buttonReleased(void)
{
	// Se comunica el estado del flanco
	uartSendStringSize(sendBufferDataFlancoAsc,sizeSendDataAsc);
}


