/******************************************************************************
 * @file    EEPROM.c
 * @brief   EEPROM driver for AT24C04 byte and 16-bit data access.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "EEPROM.h"
#include "i2c.h"
#include "SysTick.h"

#define EEPROM_TIMEOUT_MS    10U

/******************************************************************************
 * @brief Waits until the EEPROM completes its internal write cycle.
 *
 * @param slaveaddr EEPROM slave address.
 *
 * @retval 1 EEPROM is ready.
 * @retval 0 Timeout occurred.
 ******************************************************************************/
uint8_t EEPROM_WaitReady(uint8_t slaveaddr)
{
    uint32_t start = SysTick_ms;

    /* Poll the EEPROM until it acknowledges or timeout occurs */
    while ((SysTick_ms - start) < EEPROM_TIMEOUT_MS)
    {
        if (I2C1_CheckDevice(slaveaddr))
        {
            return 1;
        }
    }

    return 0;
}

/******************************************************************************
 * @brief Writes one byte to the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    Data byte to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed.
 ******************************************************************************/
uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    uint8_t page_bit;
    uint8_t slave_address;
    uint8_t memory_offset;
    uint8_t buffer[2];

    /* Calculate EEPROM page, slave address, and memory offset */
    page_bit      = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;
    slave_address = 0x50U | page_bit;

    /* Prepare write buffer */
    buffer[0] = memory_offset;
    buffer[1] = data;

    /* Send address and data */
    uint8_t status = I2C1_Send(slave_address, buffer, 2);

    if (status != 1)
    {
        return status;
    }

    /* Wait until EEPROM finishes the internal write cycle */
    return EEPROM_WaitReady(slave_address);
}

/******************************************************************************
 * @brief Writes a 16-bit value to the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    16-bit data to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed.
 ******************************************************************************/
uint8_t EEPROM_WriteUint16(uint16_t address, uint16_t data)
{
    uint8_t low_byte;
    uint8_t high_byte;
    uint8_t status;

    /* Split 16-bit data into two bytes */
    low_byte  = data & 0xFFU;
    high_byte = (data >> 8) & 0xFFU;

    /* Write low byte */
    status = EEPROM_WriteByte(address, low_byte);

    if (status != 1)
    {
        return status;
    }

    /* Write high byte */
    status = EEPROM_WriteByte(address + 1, high_byte);

    if (status != 1)
    {
        return status;
    }

    return 1;
}

/******************************************************************************
 * @brief Reads one byte from the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    Pointer to store the received byte.
 *
 * @retval 1 Read successful.
 * @retval 0 Read failed.
 ******************************************************************************/
uint8_t EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    uint8_t page_bit;
    uint8_t slave_address;
    uint8_t memory_offset;

    /* Calculate EEPROM page, slave address, and memory offset */
    page_bit      = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;
    slave_address = 0x50U | page_bit;

    /* Read one byte from EEPROM */
    return I2C1_WriteRead(slave_address, &memory_offset, 1, data, 1);
}

/******************************************************************************
 * @brief Reads a 16-bit value from the EEPROM.
 *
 * @param address EEPROM memory address.
 * @param data    Pointer to store the received 16-bit value.
 *
 * @retval 1 Read successful.
 * @retval 0 Read failed.
 ******************************************************************************/
uint8_t EEPROM_ReadUint16(uint16_t address, volatile uint16_t *data)
{
    uint8_t low_byte;
    uint8_t high_byte;
    uint8_t status;
    uint16_t value;

    /* Read low byte */
    status = EEPROM_ReadByte(address, &low_byte);

    if (status != 1)
    {
        return status;
    }

    /* Read high byte */
    status = EEPROM_ReadByte(address + 1, &high_byte);

    if (status != 1)
    {
        return status;
    }

    /* Combine the two bytes into a 16-bit value */
    value = ((uint16_t)high_byte << 8) | low_byte;
    *data = value;

    return 1;
}
