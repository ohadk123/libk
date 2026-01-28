#ifndef INCLUDE_LIBK_ERRORS_H_
#define INCLUDE_LIBK_ERRORS_H_

typedef enum {
    UNCATEGORIZED_ERROR = -1,
    NO_ERROR = 0,

    // FileControl.h
    OPEN_FILE_ERR,
    SEEK_FILE_ERR,
    READ_FILE_ERR,
    FILE_TOO_BIG_ERR
} ErrCode;

#define RETURN(res)    \
    do {              \
        result = res; \
        goto defer;   \
    } while(0);

#endif // INCLUDE_LIBK_ERRORS_H_
