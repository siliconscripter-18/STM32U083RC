/*
 * motor.c
 * Motor Driver Implementation
 */

#include "motor.h"
#include "registers.h"
#include "uart.h"

/* Motor Initialization */
void Motor_Init(void)
{
		/* Enable clock for GPIOA */
		RCC_IOPENR |= (1<<GPIOAEN);

		/* Configure PA4 as output (Motor IN1) */
		GPIOA_MODER &= ~(3<<(PA4*2));
		GPIOA_MODER |=  (1<<(PA4*2));

		/* Configure PA6 as output (Motor IN2) */
		GPIOA_MODER &= ~(3<<(PA6*2));
		GPIOA_MODER |=  (1<<(PA6*2));
}

/* Motor Control */
void Motor_Forward(void)
{
		/* Set IN1 HIGH and IN2 LOW (Motor Forward) */
		GPIOA_BSRR = (1 <<PA4) | (1 << (PA6 + 16));

		UART_SendString("MOTOR FORWARD\r\n");
}

void Motor_Reverse(void)
{
		/* Set IN1 LOW and IN2 HIGH (Motor Reverse) */
		GPIOA_BSRR = (1 <<PA6) | (1 << (PA4 + 16));

		UART_SendString("MOTOR REVERSE\r\n");
}

void Motor_Stop(void)
{
		/* Set IN1 LOW and IN2 LOW (Motor Stop) */
		GPIOA_BSRR = (1 << (PA4 + 16)) | (1 << (PA6 + 16));

		UART_SendString("MOTOR STOP\r\n");
}
