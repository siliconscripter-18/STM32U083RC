
#ifndef SEVENSEG_H
#define SEVENSEG_H

#include <stdint.h>

void SevenSeg_Init(void);
void SevenSeg_Blank(void);
void SevenSeg_SetNumber(uint8_t number);
void SevenSeg_MultiplexTask(void);

#endif /* SEVENSEG_H */
