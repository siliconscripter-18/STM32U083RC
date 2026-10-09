/******************************************************************************
 * @file    fsm.c
 * @brief   Finite state machine for the Industrial Environment Monitor.
 *
 *          Manages temperature-based operating states and controls the fan,
 *          thermal alarm, and OLED status display.
 *
 *          States:
 *          SETUP          → Initial sensor acquisition
 *          STANDBY        → Normal temperature operation
 *          WARM_ALERT     → Elevated temperature operation
 *          THERMAL_ALERT  → High temperature emergency operation
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

#define WARM_TEMP       35.0f
#define THERMAL_TEMP    40.0f

#define FAN_NORMAL      250U
#define FAN_WARM        500U
#define FAN_THERMAL     750U

float humidity = 0;
float temperature = 0;

uint8_t dht_updated = 0;

uint16_t fan = 0;

uint32_t last_dht_tick = 0;


/******************************************************************************
 * @brief FSM operating states.
 ******************************************************************************/
typedef enum
{
    SETUP,
    STANDBY,
    WARM_ALERT,
    THERMAL_ALERT
} FSM_State_t;

static FSM_State_t CurrentState = SETUP;


/******************************************************************************
 * @brief Initializes the environmental monitoring FSM.
 ******************************************************************************/
void FSM_Init(void)
{
    CurrentState = SETUP;

    /* Display startup status */
    Display_UpdateStatus(0.0, 0.0, 0, "SETTING UP");
}


/******************************************************************************
 * @brief Processes the current FSM state.
 *
 *        DHT22 measurements are acquired every 3 seconds. The measured
 *        temperature determines the active operating state and fan speed.
 ******************************************************************************/
void FSM_Process(void)
{
    /* Read DHT22 every 3 seconds */
    if ((SysTick_ms - last_dht_tick) >= 3000U)
    {
        last_dht_tick = SysTick_ms;

        if (DHT22_Read(&humidity, &temperature))
        {
        	// temperature = 41.0f;   /* TEST ONLY */
        	dht_updated = 1;
        }
    }

    switch (CurrentState)
    {
        case SETUP:

            /* Wait for the first valid DHT22 reading */
            if (dht_updated)
            {
                dht_updated = 0;

                if (temperature >= THERMAL_TEMP)
                {
                    CurrentState = THERMAL_ALERT;
                }
                else if (temperature >= WARM_TEMP)
                {
                    CurrentState = WARM_ALERT;
                }
                else
                {
                    CurrentState = STANDBY;
                }
            }

            break;


        case STANDBY:

            fan = FAN_NORMAL;
            TIM3_PWM_SetDuty(fan);

            /* Update display when a new DHT22 reading is available */
            if (dht_updated)
            {
                Display_UpdateStatus(temperature, humidity, fan, "STANDBY");
                dht_updated = 0;
            }

            /* Check for temperature state transition */
            if (temperature >= THERMAL_TEMP)
            {
                CurrentState = THERMAL_ALERT;
            }
            else if (temperature >= WARM_TEMP)
            {
                CurrentState = WARM_ALERT;
            }

            break;


        case WARM_ALERT:

            fan = FAN_WARM;
            TIM3_PWM_SetDuty(fan);

            /* Update display when a new DHT22 reading is available */
            if (dht_updated)
            {
                Display_UpdateStatus(temperature, humidity, fan, "WARM ALERT");
                dht_updated = 0;
            }

            /* Check for temperature state transition */
            if (temperature >= THERMAL_TEMP)
            {
                CurrentState = THERMAL_ALERT;
            }
            else if (temperature < WARM_TEMP)
            {
                CurrentState = STANDBY;
            }

            break;


        case THERMAL_ALERT:

            /* Thermal alarm has priority over occupancy warning */
            Buzzer_ThermalAlarm();

            fan = FAN_THERMAL;
            TIM3_PWM_SetDuty(fan);

            /* Update display when a new DHT22 reading is available */
            if (dht_updated)
            {
                Display_UpdateStatus(temperature, humidity, fan, "THERMAL ALERT");
                dht_updated = 0;
            }

            /* Return to warm state when temperature falls below threshold */
            if (temperature < THERMAL_TEMP)
            {
                CurrentState = WARM_ALERT;
            }

            break;
    }
}


/******************************************************************************
 * @brief Checks whether the FSM is currently in thermal alert state.
 *
 * @return 1 if thermal alert is active, otherwise 0.
 ******************************************************************************/
uint8_t FSM_IsThermal(void)
{
    return (CurrentState == THERMAL_ALERT) ? 1U : 0U;
}
