/******************************************************************************
 * @file    EEPROM.h
 * @brief   Interface for the external EEPROM driver.
 *
 *          Declares functions for EEPROM readiness checking and byte or
 *          16-bit data read/write operations using I2C communication.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/******************************************************************************
 * @brief Waits until the EEPROM is ready to accept a new operation.
 *
 * @param slaveaddr 7-bit I2C slave address of the EEPROM.
 * @return 1U if the EEPROM is ready; otherwise, 0U.
 ******************************************************************************/
uint8_t EEPROM_WaitReady(uint8_t slaveaddr);

/******************************************************************************
 * @brief Writes one byte to the specified EEPROM memory address.
 *
 * @param address EEPROM memory address to write to.
 * @param data    Byte value to be written.
 * @return Status indicating whether the write operation succeeded.
 ******************************************************************************/
uint8_t EEPROM_WriteByte(
    uint16_t address,
    uint8_t data);

/******************************************************************************
 * @brief Writes a 16-bit value to the specified EEPROM memory address.
 *
 * @param address EEPROM starting memory address.
 * @param data    16-bit value to be written.
 * @return Status indicating whether the write operation succeeded.
 ******************************************************************************/
uint8_t EEPROM_WriteUint16(uint16_t address,uint16_t data);

/******************************************************************************
 * @brief Reads one byte from the specified EEPROM memory address.
 *
 * @param address EEPROM memory address to read from.
 * @param data    Pointer to store the byte read from EEPROM.
 * @return Status indicating whether the read operation succeeded.
 ******************************************************************************/
uint8_t EEPROM_ReadByte(uint16_t address,uint8_t *data);

/******************************************************************************
 * @brief Reads a 16-bit value from the specified EEPROM memory address.
 *
 * @param address EEPROM starting memory address.
 * @param data    Pointer to a volatile uint16_t variable to store the value.
 * @return Status indicating whether the read operation succeeded.
 ******************************************************************************/
uint8_t EEPROM_ReadUint16(uint16_t address,volatile uint16_t *data);

#endif /* EEPROM_H */
