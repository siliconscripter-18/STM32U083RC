
#include "main.h"
#include "SevenSeg.h"
#include "cmsis_os2.h"

/*
 * Segment pins:
 * PB0 = A, PB1 = B, PB2 = C, PB3 = D
 * PB4 = E, PB5 = F, PB6 = G
 *
 * Digit select:
 * PB11 = Tens digit
 * PB12 = Units digit
 *
 * Common-cathode display.
 * HIGH enables the BC547 digit transistor.
 */

#define SEGMENT_MASK (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)

#define DIGIT1_PIN     GPIO_PIN_11
#define DIGIT2_PIN     GPIO_PIN_12
#define DIGIT_MASK     (DIGIT1_PIN | DIGIT2_PIN)

static const uint8_t digitpattern[10] =
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static uint8_t tens_digit = 0;
static uint8_t ones_digit = 0;
static uint8_t active_digit = 0;
static uint32_t last_refresh_tick = 0;

/* Disable both digit transistors */
static void Digits_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, DIGIT_MASK, GPIO_PIN_RESET);
}

/* Turn off all seven segments */
void SevenSeg_Blank(void)
{
    HAL_GPIO_WritePin(GPIOB, SEGMENT_MASK, GPIO_PIN_RESET);
}


/* Initialize display outputs to a safe state.
 * CubeMX configures the GPIO pins.
 */
void SevenSeg_Init(void)
{
    Digits_Off();
    SevenSeg_Blank();

    tens_digit = 0;
    ones_digit = 0;
    active_digit = 0;
    last_refresh_tick = osKernelGetTickCount();
}


/* Store a number from 0 to 99 */
void SevenSeg_SetNumber(uint8_t number)
{
    if (number > 99U)
    {
        number = 0U;
    }

    tens_digit = number / 10U;
    ones_digit = number % 10U;
}

/* Refresh one digit whenever at least one RTOS tick has elapsed.
 * Call this function repeatedly from a running task.
 */
void SevenSeg_MultiplexTask(void)
{
    uint32_t now = osKernelGetTickCount();
    uint8_t digit;

    if ((uint32_t)(now - last_refresh_tick) < 1U)
    {
        return;
    }

    last_refresh_tick = now;

    /* Prevent ghosting */
    Digits_Off();
    SevenSeg_Blank();

    if (active_digit == 0U)
    {
        digit = tens_digit;
    }
    else
    {
        digit = ones_digit;
    }

    /* Segment pattern bits map directly to PB0-PB6 */
    HAL_GPIO_WritePin(GPIOB, (uint16_t)digitpattern[digit],GPIO_PIN_SET);

    /* Enable only the selected digit */
    if (active_digit == 0U)
    {
        HAL_GPIO_WritePin(GPIOB, DIGIT1_PIN, GPIO_PIN_SET);
        active_digit = 1U;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, DIGIT2_PIN, GPIO_PIN_SET);
        active_digit = 0U;
    }
}
