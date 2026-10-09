/******************************************************************************
 * @file    display.h
 * @brief   OLED display interface for the Industrial Environment Monitor.
 *
 *          Declares the application-level display functions used to show
 *          the startup screen, static interface layout, and live
 *          environmental monitoring data.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define SPLASH_SCREEN_TIME_MS    2000U

/******************************************************************************
 * @brief Displays the startup splash screen.
 ******************************************************************************/
void SSD1306_ShowSplash(void);

/******************************************************************************
 * @brief Displays the static OLED interface layout.
 ******************************************************************************/
void SSD1306_ShowLayout(void);

/******************************************************************************
 * @brief Updates the live environmental monitoring data on the OLED.
 *
 * @param temperature  Current temperature in degrees Celsius.
 * @param humidity     Current relative humidity in percent.
 * @param fan_duty     Current fan PWM duty value.
 * @param status       Current system status message.
 ******************************************************************************/
void Display_UpdateStatus(float temperature,
                          float humidity,
                          uint16_t fan_duty,
                          const char *status);

#endif /* DISPLAY_H */
