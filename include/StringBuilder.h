#ifndef INCLUDE_STRING_BUILDER_H_
#define INCLUDE_STRING_BUILDER_H_

#include "Errors.h"
#include "Types.h"
#include "List.h"
#include "String.h"

typedef struct {
    LIST_FIELDS(u8);
} StringBuilder;

usize SB_joinCStr(StringBuilder *dest, cstr src);
usize SB_joinCStrN(StringBuilder *dest, cstr src, usize max);
usize SB_joinStr(StringBuilder *dest, String *src);
String SB_moveToString(StringBuilder *sb);
String SB_copyToString(StringBuilder *sb);
ErrCode SB_joinEntireFile(cstr path, StringBuilder *sb);

#endif  // INCLUDE_STRING_BUILDER_H_
