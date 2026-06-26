#include "uart.h"
#include "registers.h"


	void UART_Init(void){

		   RCC_IOPENR |= (1<<GPIOAEN); //Enable clock for GPIOA

	       GPIOA_MODER &= ~(3<<(PA2*2));
	       GPIOA_MODER |=  (2<<(PA2*2)); //Configure PA2 as Alternate Function

	       GPIOA_MODER &= ~(3<<(PA3*2));
	       GPIOA_MODER |=  (2<<(PA3*2)); //Configure PA3 as Alternate Function

	       GPIOA_AFRL &= ~(0xF<<(PA2*4));
	       GPIOA_AFRL |=  (AF7<<(PA2*4)); //AF7 for USART mode, AFRL has four bit per pin by writing 7(0111)in bit 8-11 for PA2

	       GPIOA_AFRL &= ~(0xF<<(PA3*4));
	       GPIOA_AFRL |=  (AF7<<(PA3*4)); //AF7 for USART mode, AFRL has four bit per pin by writing 7(0111)in bit 15-12 for PA3


	       RCC_APBENR1 |= (1<<USART2EN); // Enable clock for USART2

	       USART2_BRR = 35; //BRR = 4 MHz / 115200 ≈ 35

	       USART2_CR1 |= (1<<TE);   // Enable Transmitter
	       USART2_CR1 |= (1<<RE);  // Enable Receiver
	       USART2_CR1 |= (1<<UE); // Enable USART2

}


	void UART_SendChar(char c){

		   while(!(USART2_ISR&(1<<TXFNF))); // Wait until TXFNF = 1 (Transmit FIFO not full)
		   USART2_TDR = c;


}

	void UART_SendString(const char *msg){

	int i=0;

		while(msg[i]!='\0'){

			 UART_SendChar(msg[i]);

			i++;

		}

}

char UART_ReadChar(void)
	{
	    while(!(USART2_ISR&(1<<RXFNE))); // Wait until RXFNE = 1 (Receive FIFO not empty)

	    return USART2_RDR;
	}


void Help(void){

		UART_SendString(
		       "L - LED ON\r\n"
		       "O - LED OFF\r\n"
		       "S - LED STATUS\r\n"
		       "B - BUTTON STATUS\r\n"
			   "F - MOTOR FORWARD\r\n"
			   "R - MOTOR REVERSE\r\n"
			   "X - MOTOR STOP\r\n"
			   "H - Help Menu\r\n"
			   "User Inputs are Case-insensitive\r\n"
		       );
	}
