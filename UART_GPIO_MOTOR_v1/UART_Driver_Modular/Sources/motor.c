#include "gpio.h"
#include "registers.h"
#include "uart.h"



void Motor_Init(void){

		RCC_IOPENR|=(1<<GPIOAEN); // enable clock for GPIOA

		GPIOA_MODER&=~(3<<(PA8*2));
		GPIOA_MODER|=(1<<(PA8*2)); //set PA8 as o/p for IN1


		GPIOA_MODER&=~(3<<(PA9*2));
		GPIOA_MODER|=(1<<(PA9*2)); //set PA9 as o/p for IN2
}

void Motor_Forward(void){

		GPIOA_ODR|=(1<<PA8);  // IN1 = HIGH,
		GPIOA_ODR&=~(1<<PA9); // IN2 = LOW (Motor Forward)

		UART_SendString("MOTOR FORWARD\r\n");
}

void Motor_Reverse(void){

		GPIOA_ODR&=~(1<<PA8); //IN1 = LOW,
		GPIOA_ODR|=(1<<PA9); //IN2 = HIGH (Motor Reverse)

		UART_SendString("MOTOR REVERSE\r\n");


}

void Motor_Stop(void){

	GPIOA_ODR&=~(1<<PA8);
	GPIOA_ODR&=~(1<<PA9); // IN1 = LOW, IN2 = LOW (Motor Stop)

	UART_SendString("MOTOR STOP\r\n");

}
