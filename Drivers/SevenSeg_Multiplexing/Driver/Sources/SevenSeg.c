
/******************************************************************************
 * @file    SevenSeg.c
 * @brief   Seven-segment display driver for a two-digit multiplexed display.
 *
 *          Configures GPIOB pins PB0-PB6 for segment control and PB11-PB12
 *          for digit selection. Supports displaying numbers from 0 to 99
 *          using a lookup table and periodic digit multiplexing.
 *
 *          Uses SysTick_ms to control the display refresh interval.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "SevenSeg.h"
#include "SysTick.h"


/******************************************************************************
 * Pin mapping:
 * PB0  -> Segment A
 * PB1  -> Segment B
 * PB2  -> Segment C
 * PB3  -> Segment D
 * PB4  -> Segment E
 * PB5  -> Segment F
 * PB6  -> Segment G
 *
 * PB11 -> Digit 1 transistor control (tens)
 * PB12 -> Digit 2 transistor control (ones)
 *
 * @note HIGH turns on each BC547 digit switch.
 ******************************************************************************/

/* Bit mask for segment pins PB0-PB6 */
#define SEGMENT_MASK   (0x7FU)

/* Digit selection pins */
#define DIGIT1_PIN     (1U << 11)
#define DIGIT2_PIN     (1U << 12)
#define DIGIT_MASK     (DIGIT1_PIN | DIGIT2_PIN)


/* Lookup table containing segment patterns for digits 0-9 */
static const uint8_t digitpattern[10] =
{
    0x3F,  /* 0 */
    0x06,  /* 1 */
    0x5B,  /* 2 */
    0x4F,  /* 3 */
    0x66,  /* 4 */
    0x6D,  /* 5 */
    0x7D,  /* 6 */
    0x07,  /* 7 */
    0x7F,  /* 8 */
    0x6F   /* 9 */
};


/* Variables used to maintain display and multiplexing state */
static uint8_t tens_digit = 0;
static uint8_t ones_digit = 0;
static uint8_t active_digit = 0;
static uint32_t last_refresh_tick = 0;


/******************************************************************************
 * @brief Disables both digit selection outputs.
 *
 *        Resets PB11 and PB12 through the GPIOB BSRR register to switch
 *        off both digit transistors before changing the segment pattern.
 ******************************************************************************/
static void Digits_Off(void)
{
    /* Reset PB11 and PB12 using the upper half of BSRR */
    GPIOB->BSRR = (uint32_t)DIGIT_MASK << 16U;
}


/******************************************************************************
 * @brief Switches off all seven-segment outputs.
 *
 *        Resets GPIOB pins PB0-PB6 to turn off all segments.
 ******************************************************************************/
void SevenSeg_Blank(void)
{
    /* Reset all segment GPIO pins */
    GPIOB->BSRR = (uint32_t)SEGMENT_MASK << 16U;
}


/******************************************************************************
 * @brief Initializes the digit selection GPIO pins.
 *
 *        Configures PB11 and PB12 as general-purpose push-pull outputs
 *        without internal pull-up or pull-down resistors.
 ******************************************************************************/
void BaseSwitch_Init(void)
{
    /* Enable the GPIOB peripheral clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Ensure both digits are off before configuration */
    Digits_Off();

    /* Configure PB11 and PB12 as general-purpose outputs */
    GPIOB->MODER &= ~((3U << (11U * 2U)) | (3U << (12U * 2U)));

    GPIOB->MODER |=  ((1U << (11U * 2U)) | (1U << (12U * 2U)));

    /* Select push-pull output mode */
    GPIOB->OTYPER &= ~DIGIT_MASK;

    /* Disable internal pull-up and pull-down resistors */
    GPIOB->PUPDR &= ~((3U << (11U * 2U)) | (3U << (12U * 2U)));
}


/******************************************************************************
 * @brief Initializes the seven-segment GPIO pins.
 *
 *        Enables the GPIOB peripheral clock and configures PB0-PB6 as
 *        general-purpose outputs for controlling segments A-G.
 *
 *        Resets the display state and initializes the multiplexing timer.
 ******************************************************************************/
void SevenSeg_Init(void)
{
    /* Enable the GPIOB peripheral clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Configure PB0-PB6 as general-purpose outputs */
    GPIOB->MODER &= ~0x00003FFFU;
    GPIOB->MODER |=  0x00001555U;

    /* Select push-pull output mode */
    GPIOB->OTYPER &= ~SEGMENT_MASK;

    /* Disable internal pull-up and pull-down resistors */
    GPIOB->PUPDR &= ~0x00003FFFU;

    /* Configure low-speed GPIO output operation */
    GPIOB->OSPEEDR &= ~0x00003FFFU;

    /* Ensure all segments are off initially */
    SevenSeg_Blank();

    /* Initialize the display digits and multiplexing state */
    tens_digit = 0;
    ones_digit = 0;
    active_digit = 0;

    /* Record the current tick as the initial refresh timestamp */
    last_refresh_tick = SysTick_ms;
}


/******************************************************************************
 * @brief Sets the number to be displayed.
 *
 *        Splits the supplied number into tens and ones digits for
 *        multiplexed display.
 *
 * @param number Number to display, in the range 0-99.
 *
 * @note Values greater than 99 are replaced with zero.
 ******************************************************************************/
void SevenSeg_SetNumber(uint8_t number)
{
    /* Replace out-of-range values with zero */
    if (number > 99U)
    {
        number = 0U;
    }

    /* Extract the tens and ones digits */
    tens_digit = number / 10U;
    ones_digit = number % 10U;
}


/******************************************************************************
 * @brief Refreshes the multiplexed seven-segment display.
 *
 *        Refreshes one digit per millisecond by selecting the appropriate
 *        segment pattern and enabling the corresponding digit transistor.
 *        Alternates between the tens and ones digits on successive calls.
 *
 * @note Requires SysTick_ms to be updated by the SysTick interrupt handler.
 *       This function must be called continuously from the main loop.
 ******************************************************************************/
void SevenSeg_MultiplexTask(void)
{
    uint32_t now = SysTick_ms;
    uint8_t digit;

    /* Wait until at least 1 ms has elapsed since the previous refresh */
    if ((uint32_t)(now - last_refresh_tick) < 1U)
    {
        return;
    }

    /* Update the refresh timestamp */
    last_refresh_tick = now;

    /* Disable both digits and blank the segments to prevent ghosting */
    Digits_Off();
    SevenSeg_Blank();

    /* Select the value for the digit being refreshed */
    if (active_digit == 0U)
    {
        digit = tens_digit;
    }

    else
    {
        digit = ones_digit;
    }

    /* Output the segment pattern for the selected number */
    GPIOB->BSRR = digitpattern[digit];

    /* Enable the selected digit and prepare to refresh the other digit */
    if (active_digit == 0U)
    {
        GPIOB->BSRR = DIGIT1_PIN;
        active_digit = 1U;
    }

    else
    {
        GPIOB->BSRR = DIGIT2_PIN;
        active_digit = 0U;
    }
}
