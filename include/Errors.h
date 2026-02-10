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
} ErrCode;

/**
 * A macro to handle returning from a function with a single exit point.
 * 'ErrCode result' variable must be defined before using this macro.
 * 'defer' label must be defined before the return statement.
 * It sets the 'result' variable to the provided value and jumps to the 'defer' label.
 * @param res The value to set 'result' to before returning.
 */
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
