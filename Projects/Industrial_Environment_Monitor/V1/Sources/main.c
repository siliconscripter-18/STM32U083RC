/******************************************************************************
 * @file    main.c
 * @brief   Main application for the Industrial Environment Monitor.
 *
 *          Initializes all peripherals and continuously processes the
 *          environmental monitoring FSM and occupancy management.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "gpio.h"
#include "SysTick.h"
#include "EXTI.h"
#include "dht22.h"
#include "display.h"
#include "ssd1306.h"
#include "i2c.h"
#include "tim1.h"
#include "tim2.h"
#include "tim3.h"
#include "fsm.h"
#include "people.h"

/******************************************************************************
 * @brief Main application entry point.
 ******************************************************************************/
int main(void)
{
    /* Initialize system tick */
    SysTick_Init();

    /* Initialize GPIO */
    GPIO_Init();

    /* Initialize external interrupts for IR sensors */
    EXTI_Init();

    /* Initialize timers */
    TIM1_Init();
    TIM2_PWM_Init();
    TIM3_PWM_Init();

    /* Initialize DHT22 sensor */
    DHT22_Init();

    /* Initialize I2C and OLED display */
    I2C1_Init();
    SSD1306_Init();

    /* Display startup screens */
    SSD1306_ShowSplash();
    SSD1306_ShowLayout();

    /* Initialize application modules */
    FSM_Init();

    while (1)
    {
        /* Process temperature-based FSM */
        FSM_Process();

        /* Process occupancy and room-light control */
        People_Process();
    }
}
