
#ifndef SOURCES_DHT22_H_
#define SOURCES_DHT22_H_

#include <stdint.h>

void DHT22_Init(void);

uint8_t DHT22_Read(float *humidity, float *temp);

#endif /* SOURCES_DHT22_H_ */
