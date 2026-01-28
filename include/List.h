#ifndef INCLUDE_LIST_LIST_H_
#define INCLUDE_LIST_LIST_H_

#include <stdint.h>
#include <stdlib.h>

#define LIST_FIELDS(type)                                                      \
    type *arr;                                                                 \
    size_t cap;                                                                \
    size_t len

#define MIN_CAP 8

#define LIST_appendSingle(l, e)                                                \
    do {                                                                       \
        if ((l)->len >= (l)->cap) {                                            \
            (l)->cap = (l)->cap < MIN_CAP ? MIN_CAP : (l)->cap * 2;            \
            (l)->arr = realloc((l)->arr, (l)->cap * sizeof(*(l)->arr));        \
        }                                                                      \
        (l)->arr[(l)->len++] = e;                                              \
    } while (0)

#define LIST_appendArr(l, a, ac)                                               \
    do {                                                                       \
        if ((l)->cap < MIN_CAP)                                                \
            (l)->cap = MIN_CAP;                                                \
        while ((l)->len + (ac) >= (l)->cap)                                    \
            (l)->cap *= 2;                                                     \
        (l)->arr = realloc((l)->arr, (l)->cap * sizeof(*(l)->arr));            \
        memcpy(&(l)->arr[(l)->len], (a), (ac) * sizeof(*(a)));                 \
        (l)->len += (ac);                                                      \
    } while (0)

#define LIST_joinLists(d, s) LIST_appendArr(d, (s)->arr, (s)->len)

#endif // INCLUDE_LIST_LIST_H_
