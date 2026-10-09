/******************************************************************************
 * @file    EEPROM.c
 * @brief   I2C driver for AT24C04N external EEPROM.
 *
 *          Provides byte and 16-bit read/write operations, including EEPROM
 *          write-cycle delays. The AT24C04N uses the I2C slave address bits
 *          to select the memory block and an 8-bit memory offset.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "EEPROM.h"
#include "i2c.h"
#include "SysTick.h"

/* EEPROM internal write-cycle delay in milliseconds. */
#define EEPROM_WRITE_DELAY_MS    5U

/******************************************************************************
 * @brief Delays execution for the specified number of milliseconds.
 *
 *          Uses the millisecond counter maintained by SysTick.
 *
 * @param delay Delay duration in milliseconds.
 ******************************************************************************/
static void EEPROM_DelayMs(uint32_t delay)
{
    uint32_t start;

    /* Capture the starting system tick. */
    start = SysTick_ms;

    /* Wait until the requested delay has elapsed. */
    while ((SysTick_ms - start) < delay)
    {
    }
}

/******************************************************************************
 * @brief Waits for the EEPROM write cycle to complete.
 *
 *          Waits for the configured write-cycle delay before another EEPROM
 *          operation. The slave address parameter is currently unused.
 *
 * @param slaveaddr I2C slave address of the EEPROM.
 * @return 1U after the delay completes.
 ******************************************************************************/
uint8_t EEPROM_WaitReady(uint8_t slaveaddr)
{
    /* Suppress the unused-parameter warning. */
    (void)slaveaddr;

    /* Allow the EEPROM internal write cycle to finish. */
    EEPROM_DelayMs(EEPROM_WRITE_DELAY_MS);

    return 1U;
}

/******************************************************************************
 * @brief Writes one byte to the specified EEPROM memory address.
 *
 *          Derives the EEPROM slave address and memory offset from the
 *          supplied address, then transmits the offset and data byte.
 *
 * @param address EEPROM memory address to write to.
 * @param data    Byte value to be written.
 * @return 1U if the I2C write succeeds; otherwise, 0U.
 ******************************************************************************/
uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    uint8_t page_bit;
    uint8_t slave_address;
    uint8_t memory_offset;
    uint8_t buffer[2];

    /* Extract the block-select bit and the 8-bit memory offset. */
    page_bit = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;

    /* Form the 7-bit I2C slave address for the selected memory block. */
    slave_address = 0x50U | page_bit;

    /* The first byte selects the memory offset; the second is the data. */
    buffer[0] = memory_offset;
    buffer[1] = data;

    /* Transmit the memory offset and data byte over I2C. */
    if (!I2C1_Send(slave_address,buffer,2U))
    {
        return 0U;
    }

    /* Wait for the EEPROM internal write cycle to complete. */
    EEPROM_DelayMs(EEPROM_WRITE_DELAY_MS);

    return 1U;
}

/******************************************************************************
 * @brief Writes a 16-bit value to consecutive EEPROM addresses.
 *
 *          Stores the least significant byte first, followed by the most
 *          significant byte at the next memory address.
 *
 * @param address Starting EEPROM memory address.
 * @param data    16-bit value to be written.
 * @return 1U if both byte writes succeed; otherwise, 0U.
 ******************************************************************************/
uint8_t EEPROM_WriteUint16(uint16_t address, uint16_t data)
{
    uint8_t low_byte;
    uint8_t high_byte;

    /* Split the 16-bit value into low and high bytes. */
    low_byte = data & 0xFFU;
    high_byte = (data >> 8) & 0xFFU;

    /* Write the low byte to the starting address. */
    if (!EEPROM_WriteByte(address, low_byte))
        return 0U;

    /* Write the high byte to the next address. */
    if (!EEPROM_WriteByte(address + 1U, high_byte))
        return 0U;

    return 1U;
}

/******************************************************************************
 * @brief Reads one byte from the specified EEPROM memory address.
 *
 *          Selects the EEPROM memory block and offset, then performs an I2C
 *          write-read transaction to retrieve one byte.
 *
 * @param address EEPROM memory address to read from.
 * @param data    Pointer to store the byte read from EEPROM.
 * @return Status returned by I2C1_WriteRead().
 ******************************************************************************/
uint8_t EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    uint8_t page_bit;
    uint8_t slave_address;
    uint8_t memory_offset;

    /* Extract the block-select bit and the 8-bit memory offset. */
    page_bit = (address >> 8) & 1U;
    memory_offset = address & 0xFFU;

    /* Form the 7-bit I2C slave address for the selected memory block. */
    slave_address = 0x50U | page_bit;

    /* Select the memory offset and read one byte from the EEPROM. */
    return I2C1_WriteRead(slave_address,&memory_offset,1U,data,1U);
}

/******************************************************************************
 * @brief Reads a 16-bit value from consecutive EEPROM addresses.
 *
 *          Reads the low byte first and the high byte from the next address,
 *          then combines both bytes into a 16-bit value.
 *
 * @param address Starting EEPROM memory address.
 * @param data    Pointer to the volatile variable that receives the value.
 * @return 1U if both byte reads succeed; otherwise, 0U.
 ******************************************************************************/
uint8_t EEPROM_ReadUint16(uint16_t address,volatile uint16_t *data)
{
    uint8_t low_byte;
    uint8_t high_byte;

    /* Read the low byte from the starting address. */
    if (!EEPROM_ReadByte(address, &low_byte))
        return 0U;

    /* Read the high byte from the next address. */
    if (!EEPROM_ReadByte(address + 1U, &high_byte))
        return 0U;

    /* Reconstruct the 16-bit value from the two bytes. */
    *data =((uint16_t)high_byte << 8) | low_byte;

    return 1U;
}
