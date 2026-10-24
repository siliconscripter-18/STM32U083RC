/*
 * uart.c
 * UART Driver Implementation
 */

#include "uart.h"
#include "registers.h"

/* UART Configuration */
void UART_Init(void)
{
		   /* Enable clock for GPIOA */
		   RCC_IOPENR |= (1<<GPIOAEN);

		   /* Configure PA2 as USART2_TX (Alternate Function) */
	       GPIOA_MODER &= ~(3<<(PA2*2));
	       GPIOA_MODER |=  (2<<(PA2*2));

	       /* Configure PA3 as USART2_RX (Alternate Function) */
	       GPIOA_MODER &= ~(3<<(PA3*2));
	       GPIOA_MODER |=  (2<<(PA3*2));

	       /* Select AF7 for USART2 */
	       GPIOA_AFRL &= ~(0xF<<(PA2*4));
	       GPIOA_AFRL |=  (AF7<<(PA2*4));

	       GPIOA_AFRL &= ~(0xF<<(PA3*4));
	       GPIOA_AFRL |=  (AF7<<(PA3*4));

	       /* Enable clock for USART2 peripheral */
	       RCC_APBENR1 |= (1<<USART2EN);

	       /* Configure BRR for 115200 bps (4 MHz USART clock) */
	       USART2_BRR = 35;

	       /* Enable USART transmitter */
	       USART2_CR1 |= (1<<TE);

	       /* Enable USART receiver */
	       USART2_CR1 |= (1<<RE);

	       /* Enable USART2 */
	       USART2_CR1 |= (1<<UE);

}

/* UART Receive Functions */
char UART_ReadChar(void)
{
    while(!(USART2_ISR & (1<<RXFNE))); // Wait until RXFNE = 1 (Receive FIFO not empty)

    return USART2_RDR;
}

void UART_ReadString(char *buffer)
{
    char c;
    int i = 0;
    int j;

    while (1)
    {
        c = UART_ReadChar();

        if (c == '\r')
        {
            buffer[i] = '\0';

            /* Convert received command to uppercase for case-insensitive comparison */
            for (j = 0; buffer[j] != '\0'; j++)
            {
                if (buffer[j] >= 'a' && buffer[j] <= 'z')
                {
                    buffer[j] -= 32;
                }
            }

            UART_SendString("\r\n");
            return;
        }

        if (i >= MAX_CMD_LEN - 1)
        {
            UART_SendString("\r\nERROR: Command too long. Press Enter and try again.\r\n");

            /* Flush remaining characters */
            do
            {
                c = UART_ReadChar();
            }
            while (c != '\r');

            buffer[0] = '\0';
            return;
        }

        buffer[i++] = c;
        UART_SendChar(c);
    }
}

/* UART Transmit Functions */
void UART_SendChar(char c)
{
	while(!(USART2_ISR & (1<<TXFNF))); // Wait until TXFNF = 1 (Transmit FIFO not full)

	USART2_TDR = c;
}

void UART_SendString(const char *msg)
{
	int i=0;

	while(msg[i]!='\0'){

		UART_SendChar(msg[i]);
		i++;

	}

}

/* UART Utility Functions */
int my_atoi(const char *str)
{
    int num = 0;
    int i = 0;

    if(str[0] < '0' || str[0] > '9')
        return -1;

    while(str[i] >= '0' && str[i] <= '9')
    {
        num = num * 10 + (str[i] - '0');
        i++;
    }

    if(str[i] != '\0')
        return -1;

    return num;
}

/* UART Help Menu */
void Help(void)
{
    UART_SendString(
        "\r\n"
        "========== COMMAND MENU ==========\r\n"
        "\r\n"
        "LD4ON       - Turn ON LD4\r\n"
        "LD4OFF      - Turn OFF LD4\r\n"
        "LD4STATUS   - LD4 Status\r\n"
        "\r\n"
        "LEDON n     - Turn ON LED (1-4)\r\n"
        "LEDOFF n    - Turn OFF LED (1-4)\r\n"
        "ALLLEDON    - Turn ON All LEDs\r\n"
        "ALLLEDOFF   - Turn OFF All LEDs\r\n"
        "\r\n"
        "BUZON       - Buzzer ON\r\n"
        "BUZOFF      - Buzzer OFF\r\n"
        "\r\n"
        "USERBTN     - On-board Button Status\r\n"
        "EXTBTN      - External Button Status\r\n"
        "\r\n"
        "MOTORFWD    - Motor Forward\r\n"
        "MOTORREV    - Motor Reverse\r\n"
        "MOTORSTOP   - Motor Stop\r\n"
        "\r\n"
        "HELP        - Display Help Menu\r\n"
    	"\r\n"
    	"Commands are Case-Insensitive\r\n"
        "==================================\r\n"
    );
}
