#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "Types.h"

typedef struct {
    usize len;
    const u8 *data;
} String;

#endif // INCLUDE_STRING_H_
