/******************************************************************************
 * @file    fsm.h
 * @brief   Finite state machine interface for the Industrial Environment
 *          Monitor.
 *
 *          Provides functions for initializing and processing the
 *          temperature-based monitoring states and checking thermal alert
 *          status.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_FSM_H_
#define SOURCES_FSM_H_

#include <stdint.h>

/******************************************************************************
 * @brief Initializes the environmental monitoring FSM.
 ******************************************************************************/
void FSM_Init(void);

/******************************************************************************
 * @brief Processes the current FSM state.
 ******************************************************************************/
void FSM_Process(void);

/******************************************************************************
 * @brief Checks whether the FSM is currently in thermal alert state.
 *
 * @return 1 if thermal alert is active, otherwise 0.
 ******************************************************************************/
uint8_t FSM_IsThermal(void);

#endif /* SOURCES_FSM_H_ */
