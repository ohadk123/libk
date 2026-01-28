#ifndef INCLUDE_LIBK_COMMON_H_
#define INCLUDE_LIBK_COMMON_H_

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

#define KB(x) (x) * 1024
#define MB(x) KB((x) * 1024)
#define GB(x) MB((x) * 1024)

#endif // INCLUDE_LIBK_COMMON_H_
