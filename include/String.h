#ifndef INCLUDE_LIBK_STRING_H_
#define INCLUDE_LIBK_STRING_H_

#include "Types.h"

typedef struct {
    u8 *data;
    usize len;
} String;

/**
 * Compares two Strings for equality.
 * @param a The first String.
 * @param b The second String.
 * @return TRUE if the Strings are equal, FALSE otherwise.
 */
Bool compareStrings(String *a, String *b);

/**
 * Compares a String with a null-terminated C string for equality.
 * @param a The String.
 * @param b The C string.
 * @return TRUE if they are equal, FALSE otherwise.
 */
Bool compareCString(String *a, cstr b);

#endif // INCLUDE_LIBK_STRING_H_
