
#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_


/* Maximum command length including '\0' */
#define MAX_CMD_LEN 10

/* UART Initialization */
void UART_Init(void);

/* UART Receive Functions */
char UART_ReadChar(void);
uint8_t UART_ProcessCommand(char *buffer);

uint8_t UART_Available(void);

/* UART Transmit Functions */
void UART_SendChar(char c);
void UART_SendString(const char *msg);


#endif /* SOURCES_UART_H_ */
