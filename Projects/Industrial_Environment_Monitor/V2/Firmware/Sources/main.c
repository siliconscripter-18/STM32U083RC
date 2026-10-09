/******************************************************************************
 * @file    main.c
 * @brief   Main application for Industrial Environment Monitor V2.
 *
 *          Initializes the peripherals and software modules, then continuously
 *          processes temperature monitoring, people detection, and UART
 *          commands. Alert events are logged to EEPROM with RTC timestamps.
 *
 *          Supported UART commands:
 *          LOG   - Display the stored alert history.
 *          CLEAR - Clear the stored alert history.
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
#include "uart.h"
#include "alertlog.h"

#include <string.h>

/******************************************************************************
 * @brief Application entry point.
 *
 *          Initializes system timing, GPIO, external interrupts, timers,
 *          the DHT22 sensor, I2C, UART, alert logging, and the OLED display.
 *          After initialization, the main loop processes the application FSM,
 *          people detection, and incoming UART commands.
 *
 * @return int Returns 0 if the application exits.
 ******************************************************************************/
int main(void)
{
    char command[MAX_CMD_LEN];

    /* Initialize the system tick and GPIO peripherals. */
    SysTick_Init();
    GPIO_Init();

    /* Initialize external interrupt handling for sensor events. */
    EXTI_Init();

    /* Initialize timers for sensor timing, buzzer control, and fan PWM. */
    TIM1_Init();
    TIM2_PWM_Init();
    TIM3_PWM_Init();

    /* Initialize the DHT22 temperature and humidity sensor interface. */
    DHT22_Init();

    /* Initialize I2C communication for the OLED, RTC, and EEPROM. */
    I2C1_Init();

    /* Initialize UART communication for command input and output. */
    UART_Init();

    /* Initialize alert history logging and its storage interface. */
    AlertLog_Init();

    /* Initialize the OLED and display the startup screen and main layout. */
    SSD1306_Init();
    SSD1306_ShowSplash();
    SSD1306_ShowLayout();

    /* Initialize the application finite state machine. */
    FSM_Init();

    /* Display the available UART commands in the terminal. */
    UART_SendString("\r\n");
    UART_SendString("========================================\r\n");
    UART_SendString(" Industrial Environment Monitor V2\r\n");
    UART_SendString("========================================\r\n");
    UART_SendString("LOG   - View alert history\r\n");
    UART_SendString("CLEAR - Clear alert history\r\n");
    UART_SendString("\r\n");

    /* Continuously process application logic and incoming commands. */
    while (1)
    {
        /* Process temperature-based operating modes and alerts. */
        FSM_Process();

        /* Process people detection and occupancy-related behavior. */
        People_Process();

        /* Check whether a complete UART command has been received. */
        if (UART_ProcessCommand(command))
        {
            /* Print the stored alert history. */
            if (strcmp(command, "LOG") == 0)
            {
                AlertLog_PrintAll();
            }
            /* Clear the stored alert history. */
            else if (strcmp(command, "CLEAR") == 0)
            {
                AlertLog_Clear();
            }
            /* Report non-empty commands that are not recognized. */
            else if (command[0] != '\0')
            {
                UART_SendString("Unknown command\r\n");
            }
        }
    }
}
