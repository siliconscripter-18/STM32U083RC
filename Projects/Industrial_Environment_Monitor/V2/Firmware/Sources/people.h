/******************************************************************************
 * @file    people.h
 * @brief   Occupancy detection and room-light control interface.
 *
 *          Provides the interface for processing IR-based entry and exit
 *          events, occupancy counting, room-light control, and occupancy
 *          warning management.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_PEOPLE_H_
#define SOURCES_PEOPLE_H_

/******************************************************************************
 * @brief Processes occupancy events and room-light control.
 ******************************************************************************/
void People_Process(void);

#endif /* SOURCES_PEOPLE_H_ */
