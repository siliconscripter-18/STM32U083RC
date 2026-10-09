/******************************************************************************
 * @file    alertlog.c
 * @brief   Alert event logging and retrieval for Industrial Environment Monitor V2.
 *
 *          Records alert events with RTC timestamps in external EEPROM.
 *          Provides functions to initialize the log, clear the log, store
 *          new events, and print the stored event history through UART.
 *
 *          Each record contains an event type followed by seconds, minutes,
 *          hours, day, date, month, and year.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "alertlog.h"
#include "EEPROM.h"
#include "rtc.h"
#include "uart.h"

#include <stdint.h>
#include <stdio.h>

/* EEPROM address where alert records begin. */
#define LOG_START_ADDRESS   0x10U

/* Each record stores one event type and seven RTC fields. */
#define LOG_RECORD_SIZE     8U

/* Maximum number of alert records supported by the log. */
#define LOG_MAX_RECORDS     62U

/* Tracks the next record position and the number of records stored. */
static uint8_t log_index = 0U;
static uint8_t log_count = 0U;

/******************************************************************************
 * @brief Returns the text name corresponding to an alert event.
 *
 * @param event Alert event identifier.
 * @return String describing the event, or "UNKNOWN" for an invalid event.
 ******************************************************************************/
static const char *AlertLog_GetEventName(AlertEvent_t event)
{
    /* Map each event identifier to its displayable name. */
    switch (event)
    {
        case WARM_START:
            return "WARM START";

        case WARM_STOP:
            return "WARM STOP";

        case THERMAL_START:
            return "THERMAL START";

        case THERMAL_STOP:
            return "THERMAL STOP";

        default:
            return "UNKNOWN";
    }
}

/******************************************************************************
 * @brief Initializes the alert logging module.
 *
 *          Resets the record index and record count.
 ******************************************************************************/
void AlertLog_Init(void)
{
    log_index = 0U;
    log_count = 0U;
}

/******************************************************************************
 * @brief Clears the alert log tracking information.
 *
 *          Resets the record index and count, then sends a confirmation
 *          message over UART.
 ******************************************************************************/
void AlertLog_Clear(void)
{
    log_index = 0U;
    log_count = 0U;

    UART_SendString("ALERT LOG CLEARED\r\n");
}

/******************************************************************************
 * @brief Stores an alert event and its RTC timestamp in EEPROM.
 *
 *          Reads the current RTC time, packs the event identifier and
 *          timestamp into an eight-byte record, and writes the record
 *          to the next EEPROM log position.
 *
 * @param event Alert event identifier to store.
 ******************************************************************************/
void AlertLog_LogEvent(AlertEvent_t event)
{
    RTC_Time_t time;
    uint8_t data[LOG_RECORD_SIZE];
    uint16_t address;
    uint8_t i;

    /* Obtain the current time and date for the event record. */
    if (!RTC_GetTime(&time))
    {
        UART_SendString("RTC READ FAIL\r\n");
        return;
    }

    /* Pack the event identifier and RTC fields into the record buffer. */
    data[0] = (uint8_t)event;
    data[1] = time.seconds;
    data[2] = time.minutes;
    data[3] = time.hours;
    data[4] = time.day;
    data[5] = time.date;
    data[6] = time.month;
    data[7] = time.year;

    /* Calculate the EEPROM address for the next record. */
    address =LOG_START_ADDRESS +((uint16_t)log_index * LOG_RECORD_SIZE);

    /* Write each byte of the record to consecutive EEPROM addresses. */
    for (i = 0U; i < LOG_RECORD_SIZE; i++)
    {
        if (!EEPROM_WriteByte(address + i, data[i]))
        {
            UART_SendString("EEPROM WRITE FAIL\r\n");
            return;
        }
    }

    /* Increment the stored record count until the maximum is reached. */
    if (log_count < LOG_MAX_RECORDS)
    {
        log_count++;
    }

    /* Advance to the next record position and wrap at the end of the log. */
    log_index++;

    if (log_index >= LOG_MAX_RECORDS)
    {
        log_index = 0U;
    }

    /* Report successful storage over UART. */
    UART_SendString("ALERT LOG STORED\r\n");
}

/******************************************************************************
 * @brief Reads and prints all tracked alert records through UART.
 *
 *          Reads each record from EEPROM, converts the event identifier
 *          to its text name, formats the timestamp, and transmits the
 *          formatted record to the UART terminal.
 ******************************************************************************/
void AlertLog_PrintAll(void)
{
    uint8_t data[LOG_RECORD_SIZE];
    uint16_t address;
    uint8_t i;
    uint8_t j;
    char buffer[80];

    /* Report when there are no records to display. */
    if (log_count == 0U)
    {
        UART_SendString("NO ALERT LOGS\r\n");
        return;
    }

    /* Print the alert history heading. */
    UART_SendString("\r\n====== ALERT LOG ======\r\n");

    /* Read and display each record currently tracked by the module. */
    for (i = 0U; i < log_count; i++)
    {
        /* Calculate the EEPROM address for the current record. */
        address =LOG_START_ADDRESS +((uint16_t)i * LOG_RECORD_SIZE);

        /* Read all bytes of the record from EEPROM. */
        for (j = 0U; j < LOG_RECORD_SIZE; j++)
        {
            if (!EEPROM_ReadByte(address + j, &data[j]))
            {
                UART_SendString("EEPROM READ FAIL\r\n");
                return;
            }
        }

        /* Format the event name and timestamp for terminal output. */
        snprintf(buffer,sizeof(buffer),"%u. %-14s %02u:%02u:%02u %02u/%02u/20%02u\r\n",
        (unsigned int)(i + 1U),
            AlertLog_GetEventName((AlertEvent_t)data[0]),
            (unsigned int)data[3],
            (unsigned int)data[2],
            (unsigned int)data[1],
            (unsigned int)data[5],
            (unsigned int)data[6],
            (unsigned int)data[7]
        );

        /* Transmit the formatted record over UART. */
        UART_SendString(buffer);
    }

    /* Print the closing separator after all records are displayed. */
    UART_SendString("=======================\r\n\r\n");
}
