/******************************************************************************
 * @file    EEPROM.h
 * @brief   AT24C04 EEPROM driver interface.
 *
 *          Provides the public interface for byte and 16-bit EEPROM access
 *          over I2C, including the application-specific addresses used to
 *          persist visitor entry and exit counts.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>


/* EEPROM memory locations used to store persistent visitor counts */
#define EEPROM_IN_COUNT_ADDR     0x0000
#define EEPROM_OUT_COUNT_ADDR    0x0004


/******************************************************************************
 * @brief Waits until the EEPROM completes its internal write cycle.
 *
 * @param slaveaddr EEPROM I2C slave address.
 *
 * @retval 1 EEPROM is ready.
 * @retval 0 Timeout occurred.
 ******************************************************************************/
uint8_t EEPROM_WaitReady(uint8_t slaveaddr);


/******************************************************************************
 * @brief Writes one byte to the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    Data byte to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed.
 ******************************************************************************/
uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data);


/******************************************************************************
 * @brief Writes a 16-bit value to the EEPROM.
 *
 * @param address EEPROM starting memory address.
 * @param data    16-bit data value to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed.
 ******************************************************************************/
uint8_t EEPROM_WriteUint16(uint16_t address, uint16_t data);


/******************************************************************************
 * @brief Reads one byte from the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    Pointer to store the received byte.
 *
 * @retval 1 Read successful.
 * @retval 0 Read failed.
 ******************************************************************************/
uint8_t EEPROM_ReadByte(uint16_t address, uint8_t *data);


/******************************************************************************
 * @brief Reads a 16-bit value from the EEPROM.
 *
 * @param address EEPROM starting memory address.
 * @param data    Pointer to store the received 16-bit value.
 *
 * @retval 1 Read successful.
 * @retval 0 Read failed.
 ******************************************************************************/
uint8_t EEPROM_ReadUint16(uint16_t address, volatile uint16_t *data);

#endif /* EEPROM_H */
