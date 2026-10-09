/******************************************************************************
 * @file    rtc.h
 * @brief   DS1307 RTC driver interface for STM32U083RC.
 *
 *          Defines the RTC time structure and function prototypes for
 *          setting and reading time and date through I2C communication.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/******************************************************************************
 * @brief Stores the time and date values read from or written to the RTC.
 *
 *          All fields use uint8_t and store decimal values.
 ******************************************************************************/
typedef struct
{
    uint8_t seconds;  /* Seconds: 0-59 */
    uint8_t minutes;  /* Minutes: 0-59 */
    uint8_t hours;    /* Hours: 0-23 in 24-hour format */
    uint8_t day;      /* Day of the week */
    uint8_t date;     /* Date of the month: 1-31 */
    uint8_t month;    /* Month: 1-12 */
    uint8_t year;     /* Year: 0-99 */
} RTC_Time_t;

/******************************************************************************
 * @brief Sets the time and date in the DS1307 RTC.
 *
 * @param time Pointer to the RTC_Time_t structure containing time and date.
 * @return 1U if the I2C write succeeds; otherwise, 0U.
 ******************************************************************************/
uint8_t RTC_SetTime(RTC_Time_t *time);

/******************************************************************************
 * @brief Reads the current time and date from the DS1307 RTC.
 *
 * @param time Pointer to the RTC_Time_t structure to store the read values.
 * @return 1U if the I2C read succeeds; otherwise, 0U.
 ******************************************************************************/
uint8_t RTC_GetTime(RTC_Time_t *time);

#endif /* RTC_H */
