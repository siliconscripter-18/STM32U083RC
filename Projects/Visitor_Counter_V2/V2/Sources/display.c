/******************************************************************************
 * @file    display.c
 * @brief   OLED display interface for the Visitor Counter application.
 *
 *          Provides the application-level display functions used to present
 *          the startup screen, static layout, and live visitor statistics.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "display.h"
#include "ssd1306.h"
#include "SysTick.h"
#include <stdio.h>

#define SPLASH_SCREEN_TIME_MS       2000U
#define DISPLAY_VALUE_BUFFER_SIZE   6U


/******************************************************************************
 * @brief Displays the startup splash screen.
 *
 *        Shows the project title for a fixed duration before the main
 *        visitor counter interface is displayed.
 ******************************************************************************/
void SSD1306_ShowSplash(void)
{
    SSD1306_Clear();

    /* Display project title */
    SSD1306_SetCursor(32, 3);
    SSD1306_DrawStringScaled("Visitor");

    SSD1306_SetCursor(32, 5);
    SSD1306_DrawStringScaled("Counter");

    /* Keep splash screen visible */
    Delay_ms(SPLASH_SCREEN_TIME_MS);
}


/******************************************************************************
 * @brief Displays the static visitor counter layout.
 *
 *        Draws the title and field labels used by the visitor counter.
 *        Dynamic values are updated separately by
 *        Display_UpdateVisitorCounts().
 ******************************************************************************/
void SSD1306_ShowLayout(void)
{
    SSD1306_Clear();

    /* Display application title */
    SSD1306_SetCursor(24, 0);
    SSD1306_DrawString("Visitor Counter");

    /* Display visitor counter field labels */
    SSD1306_SetCursor(10, 3);
    SSD1306_DrawString("IN:");

    SSD1306_SetCursor(10, 5);
    SSD1306_DrawString("OUT:");

    SSD1306_SetCursor(10, 7);
    SSD1306_DrawString("ROOM:");
}


/******************************************************************************
 * @brief Updates the visitor statistics displayed on the OLED.
 *
 * @param InCount   Total number of recorded entries.
 * @param OutCount  Total number of recorded exits.
 * @param Present   Current number of visitors inside the room.
 *
 *        The static layout remains unchanged while only the numerical
 *        values are refreshed.
 ******************************************************************************/
void Display_UpdateVisitorCounts(uint16_t InCount,
                                 uint16_t OutCount,
                                 uint16_t Present)
{
    char buffer[DISPLAY_VALUE_BUFFER_SIZE];

    /* Update total entry count */
    snprintf(buffer, sizeof(buffer), "%u", InCount);
    SSD1306_SetCursor(30, 3);
    SSD1306_DrawString(buffer);

    /* Update total exit count */
    snprintf(buffer, sizeof(buffer), "%u", OutCount);
    SSD1306_SetCursor(36, 5);
    SSD1306_DrawString(buffer);

    /* Clear previous occupancy value before displaying the new value */
    snprintf(buffer, sizeof(buffer), "%u", Present);
    SSD1306_SetCursor(42, 7);
    SSD1306_DrawString("     ");

    SSD1306_SetCursor(42, 7);
    SSD1306_DrawString(buffer);
}
