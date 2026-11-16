/******************************************************************************
 * @file    UART.h
 * @brief   UART driver function declarations and configuration.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

/* Maximum command length including null terminator */
#define MAX_CMD_LEN 10U

/******************************************************************************
 * @brief UART initialization function.
 ******************************************************************************/
void UART_Init(void);

/******************************************************************************
 * @brief UART transmit functions.
 ******************************************************************************/
void UART_SendChar(char c);
void UART_SendString(const char *msg);

#endif /* SOURCES_UART_H_ */