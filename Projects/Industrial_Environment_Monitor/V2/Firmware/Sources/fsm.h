/******************************************************************************
 * @file    fsm.h
 * @brief   Interface for the Industrial Environment Monitor V2 state machine.
 *
 *          Declares the functions used to initialize and process the finite
 *          state machine and to check whether a thermal alert is active.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef FSM_H
#define FSM_H

#include <stdint.h>

/******************************************************************************
 * @brief Initializes the environment monitor state machine.
 ******************************************************************************/
void FSM_Init(void);

/******************************************************************************
 * @brief Processes sensor readings and operating state transitions.
 ******************************************************************************/
void FSM_Process(void);

/******************************************************************************
 * @brief Checks whether the state machine is in thermal alert mode.
 *
 * @return 1U if thermal alert is active; otherwise, 0U.
 ******************************************************************************/
uint8_t FSM_IsThermal(void);

#endif /* FSM_H */
