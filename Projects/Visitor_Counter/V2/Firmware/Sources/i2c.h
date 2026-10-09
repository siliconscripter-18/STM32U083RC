/******************************************************************************
 * @file    i2c.h
 * @brief   I2C1 driver interface for the Visitor Counter application.
 *
 *          Declares the low-level I2C communication functions used by
 *          connected peripherals such as the AT24C04 EEPROM and SSD1306 OLED.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef I2C_H
#define I2C_H

#include <stdint.h>


/******************************************************************************
 * @brief Initializes I2C1 for communication over PB8 and PB9.
 *
 *        Pin mapping:
 *        PB8 -> I2C1_SCL
 *        PB9 -> I2C1_SDA
 ******************************************************************************/
void I2C1_Init(void);


/******************************************************************************
 * @brief Checks whether an I2C slave device acknowledges its address.
 *
 * @param slaveaddr 7-bit I2C slave address.
 *
 * @retval 1 Device acknowledged.
 * @retval 0 Device did not respond or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_CheckDevice(uint8_t slaveaddr);


/******************************************************************************
 * @brief Transmits a buffer of data to an I2C slave device.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param buffer    Pointer to transmit buffer.
 * @param len       Number of bytes to transmit.
 *
 * @retval 1 Transmission successful.
 * @retval 0 Transmission failed or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_Send(uint8_t slaveaddr,
                  uint8_t *buffer,
                  uint8_t len);


/******************************************************************************
 * @brief Performs a combined write-read I2C transaction.
 *
 *        Transmits data to the slave, generates a repeated START condition,
 *        and then receives the requested number of bytes.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param txbuf     Pointer to transmit buffer.
 * @param txlen     Number of bytes to transmit.
 * @param rxbuf     Pointer to receive buffer.
 * @param rxlen     Number of bytes to receive.
 *
 * @retval 1 Write-read operation successful.
 * @retval 0 Communication failed or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_WriteRead(uint8_t slaveaddr,
                       uint8_t *txbuf,
                       uint8_t txlen,
                       uint8_t *rxbuf,
                       uint8_t rxlen);


#endif /* I2C_H */
