/**
 * @file    micro-shell.c
 * @version 0.1.0
 * @brief   Bibliteoca para configuracao da micro-shell
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "micro-shell.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#if !defined(SHELL_PRINTF)

#error "Macro SHELL_PRINTF not defined"

/**
 *  Example:
 *  #SHELL_PRINTF(fmt, ...) 	xprintf(fmt, ##__VA_ARGS__);
 */

#endif

/** Size of internal buffer */
#define SHELL_BUFFER_SIZE		1024

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint16_t read_ptr = 0;
static uint8_t shell_buffer[SHELL_BUFFER_SIZE] = { 0 };
static SemaphoreHandle_t xSemaphoreShell = NULL;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/**
 * Function to check if receive a valid command
 */
static void shell_checkMessage(void);

/**
 * Function to parse the command received.
 * @param cmd String received.
 * @param size Number of bytes received.
 */
static void shell_parser (uint8_t *cmd, uint16_t size);

/**
 * Task used by micro-shell.
 * @param pvParameters NONE.
 */
static void vTaskShell(void *pvParameters);

//==============================================================================
// PRIVATE SOURCE CODE
//==============================================================================

static void vTaskShell(void *pvParameters)
{
	for(;;)
	{
		xSemaphoreTake(xSemaphoreShell, portMAX_DELAY);
		shell_checkMessage();
	}
}

static void shell_checkMessage(void)
{
	/* command arrived, has other characters? */
	if(read_ptr != 0)
	{
		shell_parser(&shell_buffer[0], read_ptr);
	}

	/* reset the buffer command */
	memset(&shell_buffer, 0, sizeof(shell_buffer));
	read_ptr = 0;
}

static void shell_parser (uint8_t *cmd, uint16_t size)
{
	uint16_t cmd_ptr = 0;
	uint16_t arg_ptr = 0;
	uint16_t cmd_size = 0;
	uint8_t command_buffer[SHELL_BUFFER_SIZE];

	uint16_t argc = 0;
	uint8_t *argv[16];

	/* copy to the root command */
	memset(&command_buffer, 0, sizeof(command_buffer));

	/* find the root command terminator (space) */
	while(cmd_ptr < size)
	{
		if(cmd[cmd_ptr] == ' ') break;
		cmd_ptr++;
	}
	cmd_size = size - cmd_ptr;


	/* extract command arguments */
	strncpy((char *)&command_buffer[0], (const char *)&cmd[cmd_ptr + 1], (size - cmd_ptr));

	/* terminates the root command */
	cmd[cmd_ptr] = 0;
	arg_ptr = 0;

#if 0
	SHELL_PRINTF("## command: %s", cmd);
	SHELL_PRINTF("## arguments: %s", command_buffer);
#endif

	SHELL_PRINTF("$ %s %s\n", cmd, command_buffer);

	/* extract the further arguments */
	while(arg_ptr < (cmd_size))
	{
		argc++;
		*(argv + (argc- 1)) = &command_buffer[arg_ptr];

		/* find terminator */
		while(command_buffer[arg_ptr] != ' ' && (arg_ptr <1023))
		{
			arg_ptr++;
		}

		/* adds to argument list */
		command_buffer[arg_ptr] = 0;
		arg_ptr++;

#if 0
		SHELL_PRINTF("## argument no: %d : %s", argc, argv[argc-1]);
#endif

	}

	/* finds and execute the command table */
	shell_callback(cmd, argc, argv);
}

//==============================================================================
// PUBLIC SOURCE CODE
//==============================================================================

void vTaskMicroShell_init(uint16_t multi_stack_size)
{
	BaseType_t xReturned;

	/* Create Semaphore to process messages */
	xSemaphoreShell = xSemaphoreCreateBinary();
	configASSERT(xSemaphoreShell);
	vQueueAddToRegistry(xSemaphoreShell, "shellSem");

	/* Start micro-shell task */
	xReturned = xTaskCreate(vTaskShell, "tkSheel", configMINIMAL_STACK_SIZE * multi_stack_size, NULL, 3, NULL);
	configASSERT(xReturned);
}

void shell_getc(uint8_t c)
{
	if(xSemaphoreShell != NULL)
	{
		if( (c == '\n') || (c == '\r'))
		{
			configASSERT(xSemaphoreShell);

			/* handle enter key */
			xSemaphoreGive(xSemaphoreShell);

		}
		else if( (c == 0x7F) || (c == 0x08))
		{
			/* handle backspace and del keys */
			read_ptr--;
			if(read_ptr < 0) read_ptr = SHELL_BUFFER_SIZE;
			shell_buffer[read_ptr] = ' ';
		}
		else
		{
			/* store the incoming character on command circular buffer */
			shell_buffer[read_ptr] = c;
			read_ptr = (read_ptr + 1) % SHELL_BUFFER_SIZE;
		}
	}
}

void shell_isr_getc(uint8_t c, BaseType_t *pHigherPriorityTaskWoken)
{
	if(xSemaphoreShell != NULL)
	{
		if( (c == '\n') || (c == '\r'))
		{
			/* handle enter key */
			xSemaphoreGiveFromISR(xSemaphoreShell, pHigherPriorityTaskWoken);

		}
		else if( (c == 0x7F) || (c == 0x08))
		{
			/* handle backspace and del keys */
			read_ptr--;
			if(read_ptr < 0) read_ptr = (SHELL_BUFFER_SIZE - 1);
			shell_buffer[read_ptr] = ' ';
		}
		else
		{
			/* store the incoming character on command circular buffer */
			shell_buffer[read_ptr] = c;
			read_ptr = (read_ptr + 1) % SHELL_BUFFER_SIZE;
		}
	}
}

//==============================================================================
// WEAK FUNCTIONS
//==============================================================================

__weak void shell_callback(uint8_t *cmd, uint16_t argc, uint8_t **argv)
{
	(void)0;
}

