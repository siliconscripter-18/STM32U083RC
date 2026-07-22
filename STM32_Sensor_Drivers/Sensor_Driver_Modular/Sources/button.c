#include "button.h"
#include "registers.h"

/* Configure PA6 and PA7 as GPIO inputs for external push buttons
   using the internal pull-up resistors. */

void Button_Init(void)
{
	/* Enable GPIOA peripheral clock */
	RCC_IOPENR  |=  (1<<GPIOA_EN);

	/* Configure PA5 & PA6 as input */
	GPIOA_MODER &= ~(3<<(PA6*2));
	GPIOA_MODER &= ~(3<<(PA7*2));

	/* Enable internal pull-up resistor */
	GPIOA_PUPDR &= ~(3<<(PA6*2));
	GPIOA_PUPDR |=  (1<<(PA6*2));

	GPIOA_PUPDR &= ~(3<<(PA7*2));
	GPIOA_PUPDR |=  (1<<(PA7*2));

}
