/******************************************************************************
 * @file    alertlog.h
 * @brief   Interface for the alert logging module.
 *
 *          Defines alert event identifiers, the alert log data structure,
 *          and function prototypes for initializing, clearing, storing,
 *          and displaying alert history.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef ALERTLOG_H
#define ALERTLOG_H

#include <stdint.h>
#include "rtc.h"

/******************************************************************************
 * @brief Identifies the alert events recorded by the environment monitor.
 ******************************************************************************/
typedef enum
{
    WARM_START    = 1,  /* Warm alert condition begins. */
    WARM_STOP     = 2,  /* Warm alert condition ends. */
    THERMAL_START = 3,  /* Thermal alert condition begins. */
    THERMAL_STOP  = 4   /* Thermal alert condition ends. */

} AlertEvent_t;

/******************************************************************************
 * @brief Represents an alert log entry.
 *
 *          Contains the alert event type and its associated RTC timestamp.
 ******************************************************************************/
typedef struct
{
    AlertEvent_t event_type; /* Type of alert event. */
    RTC_Time_t time;         /* Time and date when the event occurred. */

} AlertLog_t;

/******************************************************************************
 * @brief Initializes the alert logging module.
 ******************************************************************************/
void AlertLog_Init(void);

/******************************************************************************
 * @brief Clears the alert log tracking information.
 ******************************************************************************/
void AlertLog_Clear(void);

/******************************************************************************
 * @brief Stores an alert event with its RTC timestamp.
 *
 * @param event Alert event identifier to record.
 ******************************************************************************/
void AlertLog_LogEvent(AlertEvent_t event);

/******************************************************************************
 * @brief Reads and prints the stored alert history through UART.
 ******************************************************************************/
void AlertLog_PrintAll(void);

#endif /* ALERTLOG_H */
