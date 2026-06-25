#include"gpio.h"

#define RCC_IOPENR  (*(volatile unsigned int*)0x4002104C)
#define GPIOA_MODER (*(volatile unsigned int*)0x50000000)
#define GPIOA_ODR   (*(volatile unsigned int*)0x50000014)


void Motor_Init(void){

		RCC_IOPENR|=(1<<0);

		GPIOA_MODER&=~(3<<16);
		GPIOA_MODER|=(1<<16); //PA8 o/p


		GPIOA_MODER&=~(3<<18);
		GPIOA_MODER|=(1<<18); //PA9 o/p
}

void Motor_Forward(void){

		GPIOA_ODR|=(1<<8);  // PA8=1 and
		GPIOA_ODR&=~(1<<9); // PA9=0 makes motor spin forward
}

void Motor_Reverse(void){

		GPIOA_ODR&=~(1<<8); // PA8=0 and
		GPIOA_ODR|=(1<<9); //  PA9=1 makes motor spin Reverse

}

void Motor_Stop(void){

	GPIOA_ODR&=~(1<<8); // PA8=0 and
	GPIOA_ODR&=~(1<<9); // PA9=0 both same makes motor stop immediately

}
