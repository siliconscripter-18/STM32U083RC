#include "registers.h"
#include "uart.h"

/* Configure USART2 for 115200 baud, 8-bit data, no parity, 1 stop bit. */
void UART_Init(void)
{
	/* Enable GPIOA and USART2 peripheral clocks */
	RCC_IOPENR |= (1<<GPIOA_EN);
	RCC_APBENR1 |= (1<<USART_EN);

	/* Configure PA2 (TX) as Alternate Function mode */
	GPIOA_MODER &= ~(3<<(PA2*2));
	GPIOA_MODER |=  (2<<(PA2*2));

	/* Configure PA3 (RX) as Alternate Function mode */
	GPIOA_MODER &= ~(3<<(PA3*2));
	GPIOA_MODER |=  (2<<(PA3*2));

	/* Select AF7 for USART2 TX */
	GPIOA_AFRL &= ~(0XF<<(PA2*4));
	GPIOA_AFRL |=  (AF7<<(PA2*4));

	/* Select AF7 for USART2 RX */
	GPIOA_AFRL &= ~(0XF<<(PA3*4));
	GPIOA_AFRL |=  (AF7<<(PA3*4));

	/* Baud rate = 115200 (BRR = 35 for current clock configuration) */
	USART_BRR = 35;

	/* Enable receiver, transmitter and USART */
	USART_CR1 |= (1<<RE);
	USART_CR1 |= (1<<TE);
	USART_CR1 |= (1<<UE);

}

void UART_SendChar(char c)
{
	/* Wait until transmit data register is empty */
	while(!(USART_ISR &(1<<TXFNF)));

	USART_TDR=c;


}

void UART_SendString(const char *msg)
{

	while (*msg != '\0')
	{
	    UART_SendChar(*msg);
	    msg++;
	}


}
