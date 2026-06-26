#include"gpio.h"
#include"uart.h"
#include "registers.h"


void LED_Init(void){

	RCC_IOPENR|=(1<<GPIOAEN); //enable clock for GPIOA

	GPIOA_MODER&=~(3<<(PA5*2));
	GPIOA_MODER|=(1<<(PA5*2)); // Configure PA5 as output

}

void LED_On(void){

	GPIOA_ODR|=(1<<PA5);
	UART_SendString("LED ON\r\n");

}

void LED_Off(void){

	GPIOA_ODR&=~(1<<PA5);
	UART_SendString("LED OFF\r\n");
}

void LED_Status(void){

	if(GPIOA_ODR&(1<<PA5)){
		UART_SendString("LED Status:ON\r\n");
	}

	else{

		UART_SendString("LED Status:OFF\r\n");
	}

}

void Button_Init(void){

	RCC_IOPENR |= (1<<GPIOCEN); //enable clock for GPIOC

	GPIOC_MODER&=~(3<<(PC13*2)); //Configure PC13 as input

	GPIOC_PUPDR&=~(3<<(PC13*2));
	GPIOC_PUPDR|=(1<<(PC13*2)); // Enable pull-up resistor for PC13 (User Button)

}

void User_Button(void){

	if(GPIOC_IDR&(1<<PC13)){

		UART_SendString("BUTTON RELEASED\r\n");
	}

	else{

		UART_SendString("BUTTON PRESSED\r\n");
	}

}



