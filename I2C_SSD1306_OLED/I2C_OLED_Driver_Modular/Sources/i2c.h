#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Initialize I2C1 on PB8/PB9
void I2C1_Init(void);

// Send data over I2C1 to a slave address
uint8_t I2C1_Send(uint8_t addr, uint8_t *buffer, uint8_t len);

#endif // I2C_H
