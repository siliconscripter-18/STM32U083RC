/******************************************************************************
 * @file    font5x7.h
 * @brief   Standard ASCII 5x7 font table interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef FONT5X7_H
#define FONT5X7_H

#include <stdint.h>

/* Standard ASCII 5x7 font table
 * Character range: ASCII 32 (' ') to ASCII 126 ('~')
 */
extern const uint8_t font5x7[][5];

#endif /* FONT5X7_H */
