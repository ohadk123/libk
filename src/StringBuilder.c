#include "StringBuilder.h"
#include <string.h>

ErrCode joinByte(StringBuilder *dest, u8 byte) {
    if (dest == NULL) return NULLPTR_ERR;

    if (dest->len >= dest->cap) {
        dest->cap = dest->cap < MIN_CAP ? MIN_CAP : dest->cap * 2;
        dest->arr = realloc(dest->arr, dest->cap * sizeof(*(dest->arr)));
        if (dest->arr == NULL) return ALLOC_ERR;
    }

    dest->arr[dest->len++] = byte;
    return 1;
}

ErrCode joinBytes(StringBuilder *dest, const u8 *src, usize count) {
    if (dest->cap < MIN_CAP) dest->cap = MIN_CAP;

    while (dest->len + count >= dest->cap) dest->cap *= 2;
    dest->arr = realloc(dest->arr, dest->cap * sizeof(*dest->arr));
    if (dest->arr == NULL) return ALLOC_ERR;

    memcpy(&dest->arr[dest->len], src, count * sizeof(*src));
    dest->len += count;

    return NO_ERR;
}

ErrCode joinCStringN(StringBuilder *dest, cstr src, usize max) { return joinBytes(dest, (const u8 *)src, max); }

ErrCode joinCString(StringBuilder *dest, cstr src) { return joinCStringN(dest, src, strlen(src)); }

ErrCode joinString(StringBuilder *dest, String *src) { return joinBytes(dest, src->data, src->len); }

ErrCode joinStringSlice(StringBuilder *dest, String *src, usize start, usize end) {
    if (start >= src->len || end > src->len || start >= end) return OUT_OF_BOUNDS_ERR;
    return joinBytes(dest, &src->data[start], end - start);
}

ErrCode joinEntireFile(StringBuilder *dest, cstr path) {
    if (dest == NULL || path == NULL) return NULLPTR_ERR;
    ErrCode result = NO_ERR;

    FILE *fp = fopen(path, "r");
    if (fp == NULL) return OPEN_FILE_ERR;

    if (fseek(fp, 0, SEEK_SET) != 0) RETURN(SEEK_FILE_ERR);
    i32 start = ftell(fp);
    if (start == -1) RETURN(SEEK_FILE_ERR);
    if (fseek(fp, 0, SEEK_END) != 0) RETURN(SEEK_FILE_ERR);
    i32 end = ftell(fp);
    if (end == -1) RETURN(SEEK_FILE_ERR);
    if (end < start) RETURN(SEEK_FILE_ERR);
    usize count = end - start;
    if (fseek(fp, 0, SEEK_SET) != 0) RETURN(SEEK_FILE_ERR);

    if ((dest->arr = realloc(dest->arr, dest->cap + count)) == NULL) RETURN(ALLOC_ERR);
    dest->cap += count;

    if (fread(dest->arr + dest->len, 1, count, fp) < count) RETURN(READ_FILE_ERR);
    dest->len += count;

defer:
    fclose(fp);
    return result;
}

String moveToString(StringBuilder *src) {
    joinByte(src, '\0');

    String result = {.data = src->arr, .len = src->len};
    src->arr = NULL;
    src->len = 0;
    src->cap = 0;
    return result;
}

String copyToString(StringBuilder *src) {
    String result = {0};
    if (src->len == 0) return result;

    result.data = malloc(src->len + 1);
    if (result.data == NULL) {
        result.len = 0;
        return result;
    }

    memcpy(result.data, src->arr, src->len);
    result.len = src->len;
    result.data[result.len] = '\0';
    return result;
}
