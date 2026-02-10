#ifndef INCLUDE_LIBK_LIST_H_
#define INCLUDE_LIBK_LIST_H_

#include "Types.h"
#include <stdlib.h>

#define LIST_FIELDS(type)                                                                                              \
    type *arr;                                                                                                         \
    usize cap;                                                                                                        \
    usize len

#define MIN_CAP 8

/**
 * Appends a single element to the end of the list.
 * @param l The list to append to.
 * @param e The element to append.
 */
#define appendSingle(l, e)                                                                                        \
    do {                                                                                                               \
        if ((l)->len >= (l)->cap) {                                                                                    \
            (l)->cap = (l)->cap < MIN_CAP ? MIN_CAP : (l)->cap * 2;                                                    \
            (l)->arr = realloc((l)->arr, (l)->cap * sizeof(*(l)->arr));                                                \
        }                                                                                                              \
        (l)->arr[(l)->len++] = e;                                                                                      \
    } while (0)

/**
 * Appends multiple elements to the end of the list.
 * @param l The list to append to.
 * @param a The array of elements to append.
 * @param ac The number of elements to append.
 */
#define appendArr(l, a, ac)                                                                                       \
    do {                                                                                                               \
        if ((l)->cap < MIN_CAP) (l)->cap = MIN_CAP;                                                                    \
        while ((l)->len + (ac) >= (l)->cap) (l)->cap *= 2;                                                             \
        (l)->arr = realloc((l)->arr, (l)->cap * sizeof(*(l)->arr));                                                    \
        memcpy(&(l)->arr[(l)->len], (a), (ac) * sizeof(*(a)));                                                         \
        (l)->len += (ac);                                                                                              \
    } while (0)

/**
 * Joins two lists together by appending all elements from the source list to the destination list.
 * @param d The destination list.
 * @param s The source list.
 */
#define joinLists(d, s) LIST_appendArr(d, (s)->arr, (s)->len)

#endif // INCLUDE_LIBK_LIST_H_
