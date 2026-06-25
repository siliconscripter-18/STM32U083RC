#include"gpio.h"
#include"uart.h"

#define RCC_IOPENR  (*(volatile unsigned int*)0x4002104C)
#define GPIOA_MODER (*(volatile unsigned int*)0x50000000)
#define GPIOC_MODER (*(volatile unsigned int*)0x50000800)
#define GPIOC_IDR   (*(volatile unsigned int*)0x50000810)
#define GPIOC_PUPDR (*(volatile unsigned int*)0x5000080c)
#define GPIOA_ODR   (*(volatile unsigned int*)0x50000014)



#define GPIOAEN 0

void LED_Init(void){

	RCC_IOPENR|=(1<<0);

	GPIOA_MODER&=~(3<<10);
	GPIOA_MODER|=(1<<10);

}

void LED_On(void){

	GPIOA_ODR|=(1<<5);
	UART_SendString("LED ON\r\n");

}

void LED_Off(void){

	GPIOA_ODR&=~(1<<5);
	UART_SendString("LED OFF\r\n");
}

void LED_Status(void){

	if(GPIOA_ODR&(1<<5)){
		UART_SendString("LED ON\r\n");
	}

	else{

		UART_SendString("LED OFF\r\n");
	}

}

void Button_Init(void){

	RCC_IOPENR |= (1<<2);

	GPIOC_MODER&=~(3<<26);

	GPIOC_PUPDR&=~(3<<26);
	GPIOC_PUPDR|=(1<<26);

}

void User_Button(void){

	if(GPIOC_IDR&(1<<13)){

		UART_SendString("BUTTON RELEASED\r\n");
	}

	else{

		UART_SendString("BUTTON PRESSED\r\n");
	}

}



