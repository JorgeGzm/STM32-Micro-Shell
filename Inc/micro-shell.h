/**
 * @file    micro-shell.h
 * @version 0.1.0
 * @brief   Biblioteca para o uso do micro-shell
 */

#ifndef _MICRO_SHELL_H
#define _MICRO_SHELL_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include <stdint.h>

//==============================================================================
// PUBLIC DEFINITIONS
//==============================================================================

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

//==============================================================================
// PUBLIC VARIABLES
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

/**
 * Function to create the task micro-shell
 * @param multi_stack_size value multiplied by configMINIMAL_STACK_SIZE
 */
void vTaskMicroShell_init(uint16_t multi_stack_size);

/**
 * Function to receive a string
 * @param Buf String
 * @param Len Size of string
 */
void shell_gets(uint8_t* Buf, uint32_t Len);

/**
 * Function to receive one byte
 * @param c byte received.
 */
void shell_getc(uint8_t c);

/**
 * Function to receive one byte from an interrupt.
 * @param c Byte received.
 * @param pHigherPriorityTaskWoken Woken of task.
 */
void shell_isr_getc(uint8_t c, BaseType_t *pHigherPriorityTaskWoken);

/**
 *
 * @param type
 * @param s
 * @param size
 * @param numToInitShow
 * @return
 */
HAL_StatusTypeDef shell_outPutFormat(const uint8_t *type, const uint8_t *s, uint16_t size, uint16_t numToInitShow);

//==============================================================================
// WEAK FUNCTIONS
//==============================================================================

/**
 * Weak function to handle received command.
 * @param cmd String raw.
 * @param argc Number of arguments.
 * @param argv String with arguments.
 */
void shell_callback(uint8_t *cmd, uint16_t argc, uint8_t **argv);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* _MICRO_SHELL_H */
