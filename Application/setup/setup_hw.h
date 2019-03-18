/**
 * @file    micro-shell.h
 * @version 0.1.0
 * @brief   Biblioteca para o uso do micro-shell
 */

#ifndef _SETUP_HW_H
#define _SETUP_HW_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdarg.h>

//==============================================================================
// PUBLIC DEFINITIONS
//==============================================================================

#define CRLF						"\r\n"
#define BKPT()		  				asm("BKPT #0")
#define SHELL_PRINTF(fmt, ...) 		xprintf(fmt, ##__VA_ARGS__); xprintf("%s", CRLF)

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void setup_init(void);
void xprintf(char * format, ...);
void serial_rx_init(void);
uint8_t serial_get_data(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* _SETUP_HW_H */
