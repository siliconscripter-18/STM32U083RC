/******************************************************************************
 * @file    IWDG.h
 * @brief   Independent Watchdog (IWDG) driver function declarations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_IWDG_H_
#define SOURCES_IWDG_H_

/******************************************************************************
 * @brief IWDG initialization and refresh functions.
 ******************************************************************************/
void IWDG_Init(void);

void IWDG_Refresh(void);

#endif /* SOURCES_IWDG_H_ */