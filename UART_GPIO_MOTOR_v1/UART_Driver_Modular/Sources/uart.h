#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

void UART_Init(void);

void UART_SendChar(char c);

void UART_SendString(char msg[]);

char UART_ReadChar(void);

void Help(void);

#endif /* SOURCES_UART_H_ */
