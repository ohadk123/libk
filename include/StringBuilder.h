#ifndef INCLUDE_LIBK_STRINGBUILDER_
#define INCLUDE_LIBK_STRINGBUILDER_

#include "List.h"
#include "Errors.h"
#include "String.h"

typedef struct {
    LIST_FIELDS(u8);
} StringBuilder;

/**
 * Appends a single byte to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param byte The byte to append.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinByte(StringBuilder *dest, u8 byte);

/**
 * Appends multiple bytes to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param src The source byte array to append from.
 * @param count The number of bytes to append.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinBytes(StringBuilder *dest, const u8 *src, usize count);

/**
 * Appends a null-terminated C string to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param src The C string to append.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinCString(StringBuilder *dest, cstr src);

/**
 * Appends up to 'max' characters from a C string to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param src The C string to append from.
 * @param max The maximum number of characters to append.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinCStringN(StringBuilder *dest, cstr src, usize max);

/**
 * Appends a String to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param src The String to append.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinString(StringBuilder *dest, String *src);

/**
 * Appends a slice of a String to the end of the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param src The source String to append from.
 * @param start The starting index of the slice (inclusive).
 * @param end The ending index of the slice (exclusive).
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinStringSlice(StringBuilder *dest, String*src, usize start, usize end);

/**
 * Reads the entire contents of a file and appends it to the StringBuilder.
 * @param dest The StringBuilder to append to.
 * @param path The path to the file to read.
 * @return An ErrCode indicating success or failure.
 */
ErrCode joinEntireFile(StringBuilder *dest, cstr path);

/**
 * Moves the contents of the StringBuilder to a String.
 * The StringBuilder will clear its contents after the move.
 * Adds a null terminator at the end.
 * @param src The StringBuilder to convert.
 * @return A String containing the contents of the StringBuilder.
 */
String moveToString(StringBuilder *src);

/**
 * Copies the contents of the StringBuilder to a new String.
 * The StringBuilder remains unchanged.
 * Adds a null terminator at the end.
 * @param src The StringBuilder to copy from.
 * @return A String containing a copy of the contents of the StringBuilder.
 */
String copyToString(StringBuilder *src);

#endif  // INCLUDE_LIBK_STRINGBUILDER_
