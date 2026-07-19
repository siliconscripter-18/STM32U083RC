#include "button.h"
#include "registers.h"

/* Configure PC13 as a GPIO input with an internal pull-up resistor. */
void Button_Init(void)
{
	/* Enable GPIOC peripheral clock */
	RCC_IOPENR  |=  (1<<GPIOC_EN);

	/* Configure PC13 as input */
	GPIOC_MODER &= ~(3<<(PC13*2));

	/* Enable internal pull-up resistor */
	GPIOC_PUPDR &= ~(3<<(PC13*2));
	GPIOC_PUPDR |=  (1<<(PC13*2));

}
