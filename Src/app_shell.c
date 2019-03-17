/**
 * @file    app_shell.c
 * @author  Jorge Guzman
 * @date    Jul 15, 2017
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa a callback do micro-shell
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "app_shell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

typedef struct
{
	uint8_t wifi_name[20];
	uint8_t passwd[20];
	uint8_t host[50];
	uint16_t port;
}Rede_t;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

Rede_t config;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// SOURCE CODE
//==============================================================================

//rede -w
void shell_callback(uint8_t *cmd, uint16_t argc, uint8_t **argv)
{
	HAL_StatusTypeDef err = HAL_ERROR;

	if (strcmp((const char *)"help", (const char *)cmd) == 0)
	{
		SHELL_PRINTF("Supported commands:");
		SHELL_PRINTF("-> rede -w WIFI_NAME -pwd PASSWORD");
		SHELL_PRINTF("-> connect -h HOST -p PORT\n");
		err = HAL_OK;
	}
	else if (strcmp((const char *)"rede", (const char *)cmd) == 0)
	{
		if(argc == 4)
		{
			if (strcmp((const char *)"-w", (const char *)argv[0]) == 0 &&
				strcmp((const char *)"-pwd", (const char *)argv[2]) == 0)
			{
				snprintf((char*)config.wifi_name, 20, "%s", argv[1]);
				snprintf((char*)config.passwd, 20, "%s", argv[3]);
				err = HAL_OK;
			}
		}
	}
	else if (strcmp((const char *)"connect", (const char *)cmd) == 0)
	{
		if(argc == 4)
		{
			if (strcmp((const char *)"-h", (const char *)argv[0]) == 0 &&
			    strcmp((const char *)"-p", (const char *)argv[2]) == 0)
			{
				snprintf((char*)config.host, 20, "%s", argv[1]);
				config.port = atoi((const char *)argv[3]);
				err = HAL_OK;
			}
		}
	}
	else if (strcmp((const char *)"show", (const char *)cmd) == 0)
	{
		if(argc == 1)
		{
			if (strcmp((const char *)"-cfg", (const char *)argv[0]) == 0)
			{
				SHELL_PRINTF("======== CONFIGS ========");
				SHELL_PRINTF("wifi name: %s", config.wifi_name);
				SHELL_PRINTF("password: %s", config.passwd);
				SHELL_PRINTF("host: %s", config.host);
				SHELL_PRINTF("port: %d", config.port);
				err = HAL_OK;
			}
		}
	}
	else
	{
		SHELL_PRINTF("(X) command not found");
	}

	if(err == HAL_ERROR)
	{
		SHELL_PRINTF("(X) Sintax error");
	}

}

