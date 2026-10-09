#ifndef SOURCES_DHT22_H_
#define SOURCES_DHT22_H_

#include <stdint.h>

/* DHT22 data pin: PA10 */
#define DHT22_PIN    10U

/******************************************************************************
 * @brief Initializes GPIO required for DHT22 communication.
 ******************************************************************************/
void DHT22_Init(void);

/******************************************************************************
 * @brief Reads temperature and humidity from the DHT22 sensor.
 *
 * @param humidity Pointer to store relative humidity in percent.
 * @param temp     Pointer to store temperature in degrees Celsius.
 *
 * @return 1 if the reading is valid, otherwise 0.
 ******************************************************************************/
uint8_t DHT22_Read(float *humidity, float *temp);

#endif /* SOURCES_DHT22_H_ */
