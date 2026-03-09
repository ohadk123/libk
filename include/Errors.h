#ifndef INCLUDE_LIBK_ERRORS_H_
#define INCLUDE_LIBK_ERRORS_H_

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

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

#define TODO(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: TODO: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);                                  \
        abort();                                                                                                       \
    } while (0)

#define ILLEGAL(cond, fmt, ...)                                                                                        \
    do {                                                                                                               \
        if (cond) {                                                                                                    \
            fprintf(stderr, "Illegal use of function %s: " fmt "\n", __func__, ##__VA_ARGS__);                         \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

#define UNIMPLEMENTED(fmt, ...)                                                                                        \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: unimplemented: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);                         \
        abort();                                                                                                       \
    } while (0)

#define UNREACHABLE(fmt, ...)                                                                                          \
    do {                                                                                                               \
        fprintf(stderr, "%s:%d: unreachable: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);                           \
        abort();                                                                                                       \
    } while (0)

#define BT_SIZE 200
#define BACKTRACE()                                                                                                    \
    do {                                                                                                               \
        void *__bt_array[BT_SIZE];                                                                                     \
        size_t __bt_size;                                                                                              \
        __bt_size = backtrace(__bt_array, BT_SIZE);                                                                    \
        char **__bt__strings = backtrace_symbols(__bt_array, __bt_size);                                               \
        printf("Backtrace (%zd frames):\n", __bt_size);                                                                    \
        for (size_t i = 0; i < __bt_size; i++) {                                                                       \
            printf("%s\n", __bt__strings[i]);                                                                              \
        }                                                                                                              \
        free(__bt__strings);                                                                                           \
    } while (0);

#endif // INCLUDE_LIBK_ERRORS_H_
