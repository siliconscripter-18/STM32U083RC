/******************************************************************************
 * @file    EEPROM.c
 * @brief   AT24C04 EEPROM driver for byte and 16-bit data access.
 *
 *          Provides EEPROM read/write operations over I2C, including device
 *          ready polling after write cycles and handling of the AT24C04
 *          memory address bit through the I2C slave address.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "EEPROM.h"
#include "i2c.h"
#include "SysTick.h"

#define EEPROM_TIMEOUT_MS    10U


/******************************************************************************
 * @brief Waits for the EEPROM to complete its internal write cycle.
 *
 *          The AT24C04 does not acknowledge I2C communication while its
 *          internal write cycle is in progress. This function repeatedly
 *          polls the device until it responds or the timeout expires.
 *
 * @param slaveaddr EEPROM I2C slave address.
 *
 * @retval 1 EEPROM is ready.
 * @retval 0 Timeout occurred.
 ******************************************************************************/
uint8_t EEPROM_WaitReady(uint8_t slaveaddr)
{
    uint32_t start = SysTick_ms;

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
 *          The AT24C04 uses the upper memory address bit as part of the
 *          device address. The remaining eight address bits are transmitted
 *          as the memory offset.
 *
 * @param address EEPROM memory address.
 * @param data    Data byte to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed or EEPROM did not become ready before timeout.
 ******************************************************************************/
uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    uint8_t page_bit;
    uint8_t slave_address;
    uint8_t memory_offset;
    uint8_t buffer[2];
    uint8_t status;

    /* Calculate device address bit and memory offset */
    page_bit      = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;
    slave_address = 0x50U | page_bit;

    /* First byte is the EEPROM memory offset, second is the data */
    buffer[0] = memory_offset;
    buffer[1] = data;

    /* Send memory offset and data */
    status = I2C1_Send(slave_address, buffer, 2);

    if (status != 1)
    {
        return status;
    }

    /* Wait for the EEPROM internal write cycle to complete */
    return EEPROM_WaitReady(slave_address);
}


/******************************************************************************
 * @brief Writes a 16-bit value to the EEPROM.
 *
 *          The value is stored as two consecutive bytes in little-endian
 *          order: low byte followed by high byte.
 *
 * @param address EEPROM starting memory address.
 * @param data    16-bit data value to write.
 *
 * @retval 1 Write successful.
 * @retval 0 Write failed.
 ******************************************************************************/
uint8_t EEPROM_WriteUint16(uint16_t address, uint16_t data)
{
    uint8_t low_byte;
    uint8_t high_byte;
    uint8_t status;

    /* Split the 16-bit value into low and high bytes */
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
 *          A write-read I2C transaction is used: the memory offset is first
 *          transmitted, followed by a repeated-start read of one byte.
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

    /* Calculate device address bit and memory offset */
    page_bit      = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;
    slave_address = 0x50U | page_bit;

    /* Set memory address and read one byte */
    return I2C1_WriteRead(slave_address,
                          &memory_offset,
                          1,
                          data,
                          1);
}


/******************************************************************************
 * @brief Reads a 16-bit value from the EEPROM.
 *
 *          Two consecutive bytes are read and combined into a 16-bit value.
 *          The data is reconstructed in the same little-endian format used
 *          by EEPROM_WriteUint16().
 *
 * @param address EEPROM starting memory address.
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

    /* Reconstruct the original 16-bit value */
    value = ((uint16_t)high_byte << 8) | low_byte;

    *data = value;

    return 1;
}
