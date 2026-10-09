/******************************************************************************
 * @file    display.c
 * @brief   OLED display interface for the Industrial Environment Monitor.
 *
 *          Provides application-level display functions for the startup
 *          screen, static interface layout, and live environmental status.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "display.h"
#include "ssd1306.h"
#include "SysTick.h"
#include <stdio.h>
#include <stdint.h>

#define SPLASH_SCREEN_TIME_MS       2000U
#define DISPLAY_VALUE_BUFFER_SIZE   20U

/******************************************************************************
 * @brief Displays the startup splash screen.
 *
 *        Shows the project title for a fixed duration before the main
 *        monitoring interface is displayed.
 ******************************************************************************/
void SSD1306_ShowSplash(void)
{
    SSD1306_Clear();

    /* Display project title */
    SSD1306_SetCursor(10, 2);
    SSD1306_DrawStringScaled("INDUSTRIAL");

    SSD1306_SetCursor(5, 4);
    SSD1306_DrawStringScaled("ENVIRONMENT");

    SSD1306_SetCursor(25, 6);
    SSD1306_DrawStringScaled("MONITOR");

    /* Keep splash screen visible */
    Delay_ms(SPLASH_SCREEN_TIME_MS);
}

/******************************************************************************
 * @brief Displays the static monitoring interface layout.
 *
 *        Draws the field labels used by the environmental monitor.
 *        Dynamic values are updated separately by Display_UpdateStatus().
 ******************************************************************************/
void SSD1306_ShowLayout(void)
{
    SSD1306_Clear();

    /* Display monitoring field labels */
    SSD1306_SetCursor(4, 0);
    SSD1306_DrawString("Temperature :");

    SSD1306_SetCursor(5, 2);
    SSD1306_DrawString("Humidity :");

    SSD1306_SetCursor(5, 4);
    SSD1306_DrawString("Fan :");

    SSD1306_SetCursor(5, 6);
    SSD1306_DrawString("Stat. :");
}

/******************************************************************************
 * @brief Updates the live environmental monitoring data on the OLED.
 *
 * @param temperature  Current temperature in degrees Celsius.
 * @param humidity     Current relative humidity in percent.
 * @param fan_duty     Current fan PWM duty value.
 * @param status       Current system status message.
 *
 *        The static layout remains unchanged. Only the dynamic values are
 *        refreshed when this function is called.
 ******************************************************************************/
void Display_UpdateStatus(float temperature,
                          float humidity,
                          uint16_t fan_duty,
                          const char *status)
{
    char buffer[DISPLAY_VALUE_BUFFER_SIZE];

    /* Display temperature */
    uint16_t temp_int = (uint16_t)temperature;
    uint16_t temp_dec = (uint16_t)((temperature - temp_int) * 10);

    snprintf(buffer, sizeof(buffer), "%u.%u C", temp_int, temp_dec);

    SSD1306_SetCursor(84, 0);
    SSD1306_DrawString(buffer);

    /* Display humidity */
    uint16_t hum_int = (uint16_t)humidity;
    uint16_t hum_dec = (uint16_t)((humidity - hum_int) * 10);

    snprintf(buffer, sizeof(buffer), "%u.%u %%", hum_int, hum_dec);

    SSD1306_SetCursor(70, 2);
    SSD1306_DrawString(buffer);

    /* Convert PWM duty value to displayed fan percentage */
    uint16_t fan;

    if (fan_duty == 999U)
    {
        fan = (fan_duty / 10U) + 1U;
    }
    else
    {
        fan = fan_duty / 10U;
    }

    snprintf(buffer, sizeof(buffer), "%u  %%", fan);

    /* Clear previous fan value before displaying the new value */
    SSD1306_SetCursor(40, 4);
    SSD1306_DrawString("       ");

    SSD1306_SetCursor(40, 4);
    SSD1306_DrawString(buffer);

    /* Clear previous status before displaying the new status */
    SSD1306_SetCursor(50, 6);
    SSD1306_DrawString("             ");

    SSD1306_SetCursor(50, 6);
    SSD1306_DrawString(status);
}
