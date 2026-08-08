#ifndef SOURCES_HC_SR04_H_
#define SOURCES_HC_SR04_H_

#include <stdint.h>

void Ultrasonic_Init(void);

uint8_t UltraSonicMeasure(uint16_t *distance);


#endif /* SOURCES_HC_SR04_H_ */
