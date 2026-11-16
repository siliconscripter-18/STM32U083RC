/******************************************************************************
 * @file    IWDG.c
 * @brief   Independent Watchdog (IWDG) driver for STM32U083RC.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"

/******************************************************************************
 * @brief Initializes the Independent Watchdog.
 *
 * Starts the IWDG, unlocks the prescaler and reload registers, configures
 * the prescaler to divide the LSI clock by 32, and loads the reload value.
 ******************************************************************************/
void IWDG_Init(void)
{
    /* Start Independent Watchdog */
    IWDG->KR = 0xCCCC;

    /* Unlock prescaler and reload registers */
    IWDG->KR = 0x5555;

    /* Configure prescaler to divide LSI clock by 32 */
    IWDG->PR = IWDG_PR_PR_0 | IWDG_PR_PR_1;

    /* Wait until prescaler update is complete */
    while(IWDG->SR & IWDG_SR_PVU);

    /* Configure watchdog reload value */
    IWDG->RLR = 4000;

    /* Wait until reload value update is complete */
    while(IWDG->SR & IWDG_SR_RVU);

    /* Refresh watchdog counter */
    IWDG->KR = 0xAAAA;
}

/******************************************************************************
 * @brief Refreshes the Independent Watchdog counter.
 *
 * Reloads the watchdog counter to prevent an unintended system reset.
 ******************************************************************************/
void IWDG_Refresh(void)
{
    /* Reload watchdog counter */
    IWDG->KR = 0xAAAA;
}