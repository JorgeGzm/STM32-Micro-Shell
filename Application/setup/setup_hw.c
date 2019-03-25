/**
 * @file    app_setup.h
 * @author  Jorge Guzman
 * @date    May 23, 2018
 * @version 0.1.0
 * @brief   Bibliteoca para configurar e inicializar a aplicacao.
 * @details
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include "micro-shell.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

/* O tamanho da task deve mudar mediante o tamanho da aplicacao contida na funcao "shell_callback". */
#define SHELL_MULTIPLY_TASK_SIZE  		5 		/* Tamanho da task shell = configMINIMAL_STACK_SIZE x SHELL_MULTIPLY_STACK_SIZE */

#define MAX_SIZE_BUFFER		  			512		/* Tamanho do buffer de output da serial */

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

extern UART_HandleTypeDef huart2;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint8_t serialBuff[MAX_SIZE_BUFFER] = { 0 }; /* Buffer de output da serial */

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

uint8_t dataRX;  /* byte usado para receber os dados vindos da serial via DMA */

//==============================================================================
// SOURCE CODE
//==============================================================================

void xprintf(char * format, ...)
{
	va_list args;
	uint16_t len;

	/* Formata a mensagem para ser enviada pela serial */
	va_start(args, format);
	len = vsnprintf((char *)serialBuff, MAX_SIZE_BUFFER, format, args);
	va_end (args);

	/* TX: Transmite a mensagem formata pela saida serial. */
	HAL_UART_Transmit(&huart2, serialBuff, len, HAL_MAX_DELAY);
}

void serial_rx_init(void)
{
	/* Habilita recepccao serial usando DMA Circular de 1 byte.
	 * Obs: Este metodo evita o rearme da recepcao de dados dentro da interrupcao. */
	HAL_UART_Receive_DMA(&huart2, &dataRX, 1);
}

uint8_t serial_get_data(void)
{
	return dataRX;
}

void setup_init(void)
{
	/* Inicializa recepcao de dado pela serial */
	serial_rx_init();

	/* Inicializa task shell */
	vTaskMicroShell_init(SHELL_MULTIPLY_TASK_SIZE);

	/* Envia mensagem de start do sistema */
	xprintf("%s", "Init Program...\r\n");
}

