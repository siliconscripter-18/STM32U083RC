/******************************************************************************
 * @file    fsm.c
 * @brief   Finite State Machine for Industrial Environment Monitor V2.
 *
 *          Monitors temperature and humidity using the DHT22 sensor and
 *          controls fan speed according to the temperature range.
 *          Updates the OLED display and records alert state transitions
 *          through the alert logging module.
 *
 *          States:
 *          SETUP         - Waits for the first valid sensor reading.
 *          STANDBY       - Normal temperature operating mode.
 *          WARM_ALERT    - Temperature is at or above the warm threshold.
 *          THERMAL_ALERT - Temperature is at or above the thermal threshold.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "fsm.h"
#include "SysTick.h"
#include "display.h"
#include "dht22.h"
#include "tim3.h"
#include "tim2.h"
#include "alertlog.h"

/* Temperature thresholds in degrees Celsius. */
#define WARM_TEMP       35.0f
#define THERMAL_TEMP    40.0f

/* Fan PWM duty-cycle settings: 25%, 50%, and 75%. */
#define FAN_NORMAL      250U
#define FAN_WARM        500U
#define FAN_THERMAL     750U

/* Shared sensor readings and application status variables. */
float humidity = 0.0f;
float temperature = 0.0f;
uint8_t dht_updated = 0U;
uint16_t fan = 0U;
uint32_t last_dht_tick = 0U;

/* Operating states of the environment monitor. */
typedef enum
{
    SETUP,          /* Wait for the first valid DHT22 reading. */
    STANDBY,        /* Temperature is below the warm threshold. */
    WARM_ALERT,     /* Temperature is at or above the warm threshold. */
    THERMAL_ALERT   /* Temperature is at or above the thermal threshold. */
} FSM_State_t;

/* Tracks the current application state. */
static FSM_State_t CurrentState = SETUP;

/******************************************************************************
 * @brief Initializes the environment monitor state machine.
 *
 *          Resets sensor readings and status flags, records the current
 *          system tick, and displays the initial setup status.
 ******************************************************************************/
void FSM_Init(void)
{
    CurrentState = SETUP;
    last_dht_tick = SysTick_ms;
    dht_updated = 0U;
    fan = 0U;
    humidity = 0.0f;
    temperature = 0.0f;

    /* Show the initial setup status on the OLED display. */
    Display_UpdateStatus(0.0f,0.0f,0U,"SETTING UP");
}

/******************************************************************************
 * @brief Processes sensor readings and environment monitor state transitions.
 *
 *          Reads the DHT22 sensor every 3000 ms and updates the current
 *          operating state according to the measured temperature.
 *          Each state controls fan speed, updates the OLED when new sensor
 *          data is available, and records alert transitions.
 *
 *          Thermal alert handling takes priority over warm alert handling.
 ******************************************************************************/
void FSM_Process(void)
{
    /* Read the DHT22 sensor every 3000 ms without blocking the main loop. */
    if ((SysTick_ms - last_dht_tick) >= 3000U)
    {
        last_dht_tick = SysTick_ms;

        /* Set the update flag only when the sensor read succeeds. */
        if (DHT22_Read(&humidity, &temperature))
        {
            dht_updated = 1U;
            //temperature=40.3f;
        }
    }

    /* Execute the logic associated with the current operating state. */
    switch (CurrentState)
    {
        case SETUP:

            /* Wait until the first valid sensor reading is available. */
            if (dht_updated)
            {
                dht_updated = 0U;

                /* Select the initial state based on the measured temperature. */
                if (temperature >= THERMAL_TEMP)
                {
                    CurrentState = THERMAL_ALERT;
                    AlertLog_LogEvent(THERMAL_START);
                }
                else if (temperature >= WARM_TEMP)
                {
                    CurrentState = WARM_ALERT;
                    AlertLog_LogEvent(WARM_START);
                }
                else
                {
                    CurrentState = STANDBY;
                }
            }

            break;

        case STANDBY:

            /* Run the fan at normal speed (25% duty cycle). */
            fan = FAN_NORMAL;
            TIM3_PWM_SetDuty(fan);

            /* Refresh the OLED when new sensor data is available. */
            if (dht_updated)
            {
                Display_UpdateStatus(temperature,humidity,fan,"STANDBY");

                dht_updated = 0U;
            }

            /* Transition to the appropriate alert state if temperature rises. */
            if (temperature >= THERMAL_TEMP)
            {
                CurrentState = THERMAL_ALERT;
                AlertLog_LogEvent(THERMAL_START);
            }
            else if (temperature >= WARM_TEMP)
            {
                CurrentState = WARM_ALERT;
                AlertLog_LogEvent(WARM_START);
            }

            break;

        case WARM_ALERT:

            /* Increase fan speed to 50% while the warm alert is active. */
            fan = FAN_WARM;
            TIM3_PWM_SetDuty(fan);

            /* Refresh the OLED when new sensor data is available. */
            if (dht_updated)
            {
                Display_UpdateStatus( temperature,humidity,fan,"WARM ALERT");

                dht_updated = 0U;
            }

            /* Escalate to thermal alert if the critical threshold is reached. */
            if (temperature >= THERMAL_TEMP)
            {
                AlertLog_LogEvent(WARM_STOP);

                CurrentState = THERMAL_ALERT;

                AlertLog_LogEvent(THERMAL_START);
            }
            /* Return to standby when temperature falls below the warm threshold. */
            else if (temperature < WARM_TEMP)
            {
                AlertLog_LogEvent(WARM_STOP);

                CurrentState = STANDBY;
            }

            break;

        case THERMAL_ALERT:

            /* Activate the thermal alarm while the critical state is active. */
            Buzzer_ThermalAlarm();

            /* Run the fan at 75% duty cycle during a thermal alert. */
            fan = FAN_THERMAL;
            TIM3_PWM_SetDuty(fan);

            /* Refresh the OLED when new sensor data is available. */
            if (dht_updated)
            {
                Display_UpdateStatus(temperature,humidity,fan,"THERMAL ALERT");

                dht_updated = 0U;
            }

            /* Leave thermal alert when temperature falls below the threshold. */
            if (temperature < THERMAL_TEMP)
            {
                AlertLog_LogEvent(THERMAL_STOP);

                CurrentState = WARM_ALERT;

                AlertLog_LogEvent(WARM_START);
            }

            break;

        default:

            /* Recover to the setup state if the current state is invalid. */
            CurrentState = SETUP;

            break;
    }
}

/******************************************************************************
 * @brief Checks whether the state machine is in thermal alert mode.
 *
 * @return 1U if the current state is THERMAL_ALERT; otherwise, 0U.
 ******************************************************************************/
uint8_t FSM_IsThermal(void)
{
    return (CurrentState == THERMAL_ALERT) ? 1U : 0U;
}
