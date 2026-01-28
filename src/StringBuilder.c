#include "StringBuilder.h"
#include "Common.h"
#include "Errors.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void increaseCap(StringBuilder *sb, usize count) {
    if (sb->cap - sb->len > count) return;
    sb->cap += count;
    sb->arr = realloc(sb->arr, sb->cap);
    assert(sb->arr != NULL && "NOT ENOUGH MEMORY");
}

usize SB_joinBytes(StringBuilder *sb, const u8 *src, usize count) {
    increaseCap(sb, count);
    memcpy(sb->arr + sb->len, src, count);
    sb->len += count;
    return count;
}

usize SB_joinCStr(StringBuilder *sb, const char *src) {
    usize len = strlen(src);
    return SB_joinBytes(sb, (const u8 *)src, len);
}

usize SB_joinCStrN(StringBuilder *sb, const char *src, usize max) {
    usize len = strlen(src);
    return SB_joinBytes(sb, (const u8 *)src, MIN(len, max));
}

usize SB_joinStr(StringBuilder *sb, String *src) {
    return SB_joinBytes(sb, src->data, src->len);
}

String SB_moveToString(StringBuilder *sb) {
    SB_joinCStr(sb, "\0");

    String s = {
        .len = sb->len,
        .data = sb->arr,
    };

    sb->arr = NULL;
    sb->len = 0;
    sb->cap = 0;

    return s;
}

String SB_copyToString(StringBuilder *sb) {
    usize len = sb->len;
    u8 *data = malloc(len + 1);
    memcpy((void *)data, (void *)sb->arr, len);
    data[len] = '\0';

    return (String) {
        .len = len,
        .data = data,
    };
}

#define MAX_READ MB(1)
ErrCode SB_joinEntireFile(cstr path, StringBuilder *sb) {
    ErrCode result = NO_ERROR;

    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL) RETURN(OPEN_FILE_ERR);

    i64 start, end;
    if (fseek(fp, 0, SEEK_SET) < 0) RETURN(SEEK_FILE_ERR);
    if ((start = ftell(fp)) < 0)    RETURN(SEEK_FILE_ERR);
    if (fseek(fp, 0, SEEK_END) < 0) RETURN(SEEK_FILE_ERR);
    if ((end = ftell(fp)) < 0)      RETURN(SEEK_FILE_ERR);
    usize size = end - start;
    if (size > MAX_READ) RETURN(FILE_TOO_BIG_ERR);

    if (fseek(fp, 0, SEEK_SET) < 0) RETURN(SEEK_FILE_ERR);

    increaseCap(sb, size);
    if(fread(sb->arr + sb->len, 1, size, fp) < size) RETURN(READ_FILE_ERR);
    sb->len += size;

defer:
    if (result != NO_ERROR) perror("openFile");
    fclose(fp);
    return result;
}
