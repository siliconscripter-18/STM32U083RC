/******************************************************************************
 * @file    people.c
 * @brief   Occupancy detection and room-light control.
 *
 *          Processes entry and exit events from the IR sensors, maintains
 *          the current number of people, controls the room-light output,
 *          and generates an occupancy warning when the room limit is exceeded.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "gpio.h"
#include "EXTI.h"
#include "fsm.h"
#include "tim2.h"

uint8_t people = 0U;

/******************************************************************************
 * @brief Processes occupancy events and room-light control.
 *
 *        Entry and exit events are received through EXTI flags. The current
 *        occupancy count is maintained and the room-light output is enabled
 *        whenever at least one person is present.
 *
 *        The occupancy warning is suppressed while the thermal alarm is
 *        active, giving the thermal alarm priority.
 ******************************************************************************/
void People_Process(void)
{
    /* Process entry event */
    if (S1_Flag)
    {
        S1_Flag = 0U;
        people++;
    }

    /* Process exit event */
    if (S2_Flag)
    {
        S2_Flag = 0U;

        if (people > 0U)
        {
            people--;
        }
    }

    /* Control room-light output */
    if (people >= 1U)
    {
        GPIOA->BSRR = GPIO_BSRR_BS6;
    }
    else
    {
        GPIOA->BSRR = GPIO_BSRR_BR6;
    }

    /* Occupancy warning has lower priority than thermal alarm */
    if ((FSM_IsThermal() == 0U) && (people > 6U))
    {
        Buzzer_OccupancyWarning();
    }
    else
    {
        if (FSM_IsThermal() == 0U)
        {
            Buzzer_Stop();
        }
    }
}
