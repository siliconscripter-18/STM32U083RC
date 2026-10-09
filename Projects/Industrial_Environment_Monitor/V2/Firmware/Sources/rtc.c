/******************************************************************************
 * @file    rtc.c
 * @brief   DS1307 RTC driver for STM32U083RC.
 *
 *          Provides functions to set and read time and date from the DS1307
 *          Real-Time Clock (RTC) using I2C1 communication.
 *          Time and date values are converted between decimal and BCD format.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>
#include "rtc.h"
#include "i2c.h"

/* DS1307 7-bit I2C slave address */
#define DS1307_ADDRESS 0x68U

/******************************************************************************
 * @brief Converts a decimal value into Binary-Coded Decimal (BCD) format.
 *
 * @param value Decimal value to be converted.
 * @return BCD representation of the input value.
 ******************************************************************************/
static uint8_t DecimalToBCD(uint8_t value)
{
    return ((value / 10U) << 4) | (value % 10U);
}

/******************************************************************************
 * @brief Converts a Binary-Coded Decimal (BCD) value into decimal format.
 *
 * @param value BCD value to be converted.
 * @return Decimal representation of the input value.
 ******************************************************************************/
static uint8_t BCDToDecimal(uint8_t value)
{
    return ((value >> 4) * 10U) + (value & 0x0FU);
}

/******************************************************************************
 * @brief Sets the time and date in the DS1307 RTC.
 *
 *          The register address is placed at data[0], followed by the seconds,
 *          minutes, hours, day, date, month, and year values in BCD format.
 *          The seconds register's CH bit is cleared to enable the clock.
 *          The hours value is masked for 24-hour mode.
 *
 * @param time Pointer to the RTC_Time_t structure containing time and date.
 * @return Result returned by I2C1_Send().
 ******************************************************************************/
uint8_t RTC_SetTime(RTC_Time_t *time)
{
    uint8_t data[8];

    /* Start writing from the seconds register (address 0x00). */
    data[0] = 0x00U;

    data[1] = DecimalToBCD(time->seconds) & 0x7FU; // bit 7 CH = 0: Clock runs. CH = 1: Clock is halted.
    data[2] = DecimalToBCD(time->minutes);
    data[3] = DecimalToBCD(time->hours) & 0x3FU; // bit 6=0 is 24hr.  bit 6=1 is 12hr format
    data[4] = DecimalToBCD(time->day);
    data[5] = DecimalToBCD(time->date);
    data[6] = DecimalToBCD(time->month);
    data[7] = DecimalToBCD(time->year);

    /* Transmit the register address and seven time/date bytes to the DS1307. */
    return I2C1_Send(DS1307_ADDRESS, data, 8);
}

/******************************************************************************
 * @brief Reads the current time and date from the DS1307 RTC.
 *
 *          A combined I2C write-read transaction selects the seconds register
 *          and reads seven consecutive registers into the data buffer.
 *          The returned BCD values are converted into decimal values.
 *
 * @param time Pointer to the RTC_Time_t structure to store the read values.
 * @return 1U if the I2C read succeeds; otherwise, 0U.
 ******************************************************************************/
uint8_t RTC_GetTime(RTC_Time_t *time)
{
    uint8_t reg;
    uint8_t data[7];

    /* Select the seconds register (address 0x00). */
    reg = 0x00U;

    /* Read the seven consecutive time and date registers. */
    if (I2C1_WriteRead(DS1307_ADDRESS, &reg, 1, data, 7) != 1U)
    {
        /* Return failure if the I2C transaction does not succeed. */
        return 0U;
    }

    /* Convert BCD register values to decimal and remove control bits. */
    time->seconds = BCDToDecimal(data[0] & 0x7FU);
    time->minutes = BCDToDecimal(data[1]);
    time->hours   = BCDToDecimal(data[2] & 0x3FU);
    time->day     = BCDToDecimal(data[3]);
    time->date    = BCDToDecimal(data[4]);
    time->month   = BCDToDecimal(data[5]);
    time->year    = BCDToDecimal(data[6]);

    return 1U;
}
