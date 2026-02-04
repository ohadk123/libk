#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "Errors.h"
#include "Types.h"
#include "List.h"

typedef struct {
    LIST_FIELDS(u8);
} String;

ErrCode joinByte(String *dest, u8 byte);
ErrCode joinBytes(String *dest, const u8 *src, usize count);
ErrCode joinCString(String *dest, cstr src);
ErrCode joinCStringN(String *dest, cstr src, usize max);
ErrCode joinString(String *dest, String *src);
ErrCode joinStringSlice(String *dest, String *src, usize start, usize end);
ErrCode joinEntireFile(String *dest, cstr path);

#endif  // INCLUDE_STRING_H_
