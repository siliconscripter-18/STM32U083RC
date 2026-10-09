/******************************************************************************
 * @file    i2c.h
 * @brief   I2C1 driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/* Initializes I2C1 on PB8 (SCL) and PB9 (SDA) */
void I2C1_Init(void);

/* Checks whether an I2C slave responds with ACK */
uint8_t I2C1_CheckDevice(uint8_t slaveaddr);

/* Sends data to an I2C slave device */
uint8_t I2C1_Send(uint8_t slaveaddr, uint8_t *buffer, uint8_t len);

/* Writes data, then reads data from an I2C slave device */
uint8_t I2C1_WriteRead(uint8_t slaveaddr,uint8_t *txbuf,uint8_t txlen,uint8_t *rxbuf,uint8_t rxlen);

#endif /* I2C_H */
