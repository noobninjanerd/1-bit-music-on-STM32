#ifndef INC_SONGS_H
#define INC_SONGS_H

#include "common-defines.h"
#include "notes.h"

extern int16_t melody[];
size_t sizeOfMelody(void);
uint32_t noteDurationCalc(size_t whichNote);


#endif // INC_SONGS_H