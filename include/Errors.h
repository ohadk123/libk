#ifndef INCLUDE_LIBK_ERRORS_H_
#define INCLUDE_LIBK_ERRORS_H_

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    UNCATEGORIZED_ERR = -1,
    NO_ERR = 0,

    // Pointer Errors
    ALLOC_ERR,
    NULLPTR_ERR,
    OUT_OF_BOUNDS_ERR,

    // File Errors
    OPEN_FILE_ERR,
    SEEK_FILE_ERR,
    READ_FILE_ERR,
    FILE_TOO_BIG_ERR,

    // Lexer Errors
    LEXING_ERR,
} ErrCode;

#define RETURN(res)                                                                                                    \
    do {                                                                                                               \
        result = res;                                                                                                  \
        goto defer;                                                                                                    \
    } while (0);

#define TODO(message)                                                                                                  \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message);                                             \
        abort();                                                                                                       \
    } while (0)

#define ILLEGAL(cond, cause)                                                                                           \
    do {                                                                                                               \
        if (cond) {                                                                                                    \
            fprintf(stderr, "Illegal use of function %s: %s\n", __func__, cause);                                      \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

#define UNIMPLEMENTED(message)                                                                                         \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: unimplemented: %s\n", __FILE__, __LINE__, message);                                    \
        abort();                                                                                                       \
    } while (0)

#endif // INCLUDE_LIBK_ERRORS_H_
