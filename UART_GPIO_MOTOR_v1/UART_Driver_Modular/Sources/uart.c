#include "uart.h"

#define RCC_IOPENR   (*(volatile unsigned int*)0x4002104C)
#define RCC_APBENR1  (*(volatile unsigned int*)0x40021058)

#define GPIOA_MODER  (*(volatile unsigned int*)0x50000000)
#define GPIOA_AFRL   (*(volatile unsigned int*)0x50000020)

#define USART2_CR1   (*(volatile unsigned int*)0x40004400)
#define USART2_BRR   (*(volatile unsigned int*)0x4000440C)
#define USART2_ISR   (*(volatile unsigned int*)0x4000441C)
#define USART2_TDR   (*(volatile unsigned int*)0x40004428)
#define USART2_RDR   (*(volatile unsigned int*)0x40004424)

#define GPIOAEN 0

	void UART_Init(void){

		   RCC_IOPENR |= (1<<GPIOAEN); //enables clock to GPIOA

	       GPIOA_MODER &= ~(3<<4);
	       GPIOA_MODER |=  (2<<4);   //PA2 as alternate function

	       GPIOA_MODER &= ~(3<<6);
	       GPIOA_MODER |=  (2<<6);  //PA3 as alternate function

	       GPIOA_AFRL &= ~(0xF<<8);
	       GPIOA_AFRL |=  (7<<8);   //AF7 for USART mode, AFRL has four bit per pin by writing 7(0111)in bit 8-11 for PA2

	       GPIOA_AFRL &= ~(0xF<<12);
	       GPIOA_AFRL |=  (7<<12);  //AF7 for USART mode, AFRL has four bit per pin by writing 7(0111)in bit 15-12 for PA3


	       RCC_APBENR1 |= (1<<17); //clock register - clock for USART using (bit 17) is USART


	       USART2_BRR = 35; // For current UART setup:
	       	   	   	   	   // BRR = 4000000 / 115200 ≈ 35

	       USART2_CR1 |= (1<<3);    //enable TE. bit 3 in USART2_CR1 register is Transmission enable bit
	       USART2_CR1 |= (1<<2);   // enable RE. bit 2 in USART2_CR1 register is Receiver enable bit
	       USART2_CR1 |= (1<<0);   //enable UE.  bit 0 in  USART2_CR1 register is UART enable bit

}


	void UART_SendChar(char c){

		   while(!(USART2_ISR&(1<<7)));

		   USART2_TDR = c;


}

	void UART_SendString(char msg[]){

	int i=0;

		while(msg[i]!='\0'){

			 UART_SendChar(msg[i]);

			i++;

		}

}

char UART_ReadChar(void)
	{
	    while(!(USART2_ISR&(1<<5)));

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
			   "User Inputs are Case-insensitive\r\n"
		       );
	}
