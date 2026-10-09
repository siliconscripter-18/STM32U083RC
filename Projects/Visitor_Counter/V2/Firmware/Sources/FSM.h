/******************************************************************************
 * @file    fsm.h
 * @brief   Finite state machine interface for the Visitor Counter.
 *
 *          Defines the FSM states used to identify valid entry and exit
 *          sequences and declares the functions used to initialize and
 *          process the state machine.
 *
 *          Valid sequences:
 *          S1 -> S2 : Entry
 *          S3 -> S4 : Exit
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_FSM_H_
#define SOURCES_FSM_H_

#include <stdint.h>


/**
 * @brief Visitor Counter finite state machine states.
 */
typedef enum
{
    FSM_IDLE,       /* Waiting for the first sensor event */

    FSM_ENTRY_S1,   /* S1 triggered; waiting for S2 */
    FSM_ENTRY_S2,   /* S2 triggered; waiting for S1 */

    FSM_EXIT_S3,    /* S3 triggered; waiting for S4 */
    FSM_EXIT_S4     /* S4 triggered; waiting for S3 */

} FSM_State_t;


/******************************************************************************
 * @brief Initializes the Visitor Counter state machine.
 ******************************************************************************/
void FSM_Init(void);


/******************************************************************************
 * @brief Processes pending sensor events and advances the FSM.
 ******************************************************************************/
void FSM_Process(void);


#endif /* SOURCES_FSM_H_ */
