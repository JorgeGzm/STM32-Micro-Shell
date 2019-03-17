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

#define SHELL_MULTIPLY_STACK_SIZE  		5
#define MAX_SIZE_BUFFER		  			512

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

extern UART_HandleTypeDef huart2;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint8_t serialBuff[MAX_SIZE_BUFFER] = { 0 };

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

uint8_t dataRX;

//==============================================================================
// SOURCE CODE
//==============================================================================

void xprintf(char * format, ...)
{
	va_list args;
	uint16_t len;

	va_start(args, format);
	len = vsnprintf((char *)serialBuff, MAX_SIZE_BUFFER, format, args);
	va_end (args);

	HAL_UART_Transmit(&huart2, serialBuff, len, HAL_MAX_DELAY);
}

void serial_rx_init(void)
{
	HAL_UART_Receive_DMA(&huart2, &dataRX, 1);
}

uint8_t serial_get_data(void)
{
	return dataRX;
}

void setup_init(void)
{
	serial_rx_init();

	/* Envia mensagem de start do sistema */
	xprintf("Init Program...\r\n");

	vTaskMicroShell_init(5);
}

