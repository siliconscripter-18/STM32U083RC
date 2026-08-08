/******************************************************************************
 * @file    EEPROM.h
 * @brief   EEPROM driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/* Waits until the EEPROM becomes ready */
uint8_t EEPROM_WaitReady(uint8_t slaveaddr);

/* Writes one byte to EEPROM */
uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data);

/* Writes a 16-bit value to EEPROM */
uint8_t EEPROM_WriteUint16(uint16_t address, uint16_t data);

/* Reads one byte from EEPROM */
uint8_t EEPROM_ReadByte(uint16_t address, uint8_t *data);

/* Reads a 16-bit value from EEPROM */
uint8_t EEPROM_ReadUint16(uint16_t address, volatile uint16_t *data);

#endif /* EEPROM_H */
