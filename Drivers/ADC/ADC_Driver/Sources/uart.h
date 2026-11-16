/******************************************************************************
 * @file    UART.h
 * @brief   UART driver function declarations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

/******************************************************************************
 * @brief Initializes the UART peripheral.
 ******************************************************************************/
void UART_Init(void);

/******************************************************************************
 * @brief UART transmission functions.
 ******************************************************************************/

/* Transmit a single character */
void UART_SendChar(char c);

/* Transmit a string */
void UART_SendString(const char *msg);

#endif /* SOURCES_UART_H_ */
