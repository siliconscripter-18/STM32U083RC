/******************************************************************************
 * @file    GPIO.h
 * @brief   GPIO driver function declarations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_GPIO_H_
#define SOURCES_GPIO_H_

/******************************************************************************
 * @brief GPIO initialization and LED control functions.
 ******************************************************************************/
void GPIO_Init(void);

void Toggle_LED(void);

#endif /* SOURCES_GPIO_H_ */