#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// Standard ASCII 5x7 font
// Characters 32 (' ') through 126 ('~')

// Extern declaration so other files can access the font table
extern const uint8_t font5x7[][5];

#endif // FONT_H
