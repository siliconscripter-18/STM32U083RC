/******************************************************************************
 * @file    font5x7.h
 * @brief   Standard ASCII 5x7 font table interface for the SSD1306 OLED.
 *
 *          Provides access to the 5x7 bitmap font table used by the SSD1306
 *          display driver to render printable ASCII characters.
 *
 *          Supported character range:
 *          ASCII 32 (' ') through ASCII 126 ('~')
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef FONT5X7_H
#define FONT5X7_H

#include <stdint.h>


/******************************************************************************
 * @brief Standard ASCII 5x7 font bitmap table.
 *
 *        Each character is represented by five bytes, with each byte
 *        containing one vertical column of the character bitmap.
 ******************************************************************************/
extern const uint8_t font5x7[][5];


#endif /* FONT5X7_H */
