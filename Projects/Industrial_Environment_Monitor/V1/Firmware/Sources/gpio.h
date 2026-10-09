/******************************************************************************
 * @file    gpio.h
 * @brief   GPIO interface for the Industrial Environment Monitor.
 *
 *          Provides the GPIO initialization function used to configure
 *          IR sensor inputs, motor control outputs, and the room-light
 *          test LED.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_GPIO_H_
#define SOURCES_GPIO_H_

/******************************************************************************
 * @brief Initializes GPIO pins used by the monitoring system.
 ******************************************************************************/
void GPIO_Init(void);

#endif /* SOURCES_GPIO_H_ */
