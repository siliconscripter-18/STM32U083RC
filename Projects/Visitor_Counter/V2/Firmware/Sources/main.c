/******************************************************************************
 * @file    main.c
 * @brief   Main application entry point for the Visitor Counter.
 *
 *          Initializes all required peripherals and application modules,
 *          restores persistent visitor counts from EEPROM, initializes the
 *          OLED interface, and continuously processes sensor events through
 *          the EXTI driver and visitor counter state machine.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>

#include "stm32u083xx.h"
#include "SysTick.h"
#include "EXTI.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"
#include "display.h"
#include "EEPROM.h"
#include "fsm.h"


/* Visitor statistics maintained by the application */
volatile uint16_t InCount  = 0;
volatile uint16_t OutCount = 0;
volatile uint16_t Present  = 0;


/******************************************************************************
 * @brief Main application entry point.
 *
 *          Initializes GPIO, SysTick, EXTI, I2C, OLED, and application
 *          modules. Previously stored entry and exit counts are restored from
 *          EEPROM so visitor statistics persist across power cycles.
 *
 *          The main loop continuously re-arms cleared sensors and processes
 *          sensor events through the finite state machine.
 ******************************************************************************/
int main(void)
{
    /* Initialize MCU peripherals and application interfaces */
    GPIO_Init();
    SysTick_Init();
    EXTI_Init();
    I2C1_Init();
    SSD1306_Init();


    /* Restore persistent visitor statistics from EEPROM */
    EEPROM_ReadUint16(EEPROM_IN_COUNT_ADDR, &InCount);
    EEPROM_ReadUint16(EEPROM_OUT_COUNT_ADDR, &OutCount);

    /* Calculate current room occupancy */
    Present = InCount - OutCount;


    /* Initialize and display the OLED user interface */
    SSD1306_ShowSplash();
    SSD1306_ShowLayout();
    Display_UpdateVisitorCounts(InCount, OutCount, Present);


    /* Initialize the visitor counter state machine */
    FSM_Init();


    /*
     * Main application loop.
     *
     * EXTI_Process() re-arms sensors after their beams become clear.
     * FSM_Process() interprets sensor events and updates visitor statistics.
     */
    while (1)
    {
        EXTI_Process();
        FSM_Process();
    }
}
