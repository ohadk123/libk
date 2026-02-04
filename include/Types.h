#ifndef INCLUDE_LIBK_TYPES_H_
#define INCLUDE_LIBK_TYPES_H_

typedef __UINT8_TYPE__  u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

#define U8_MAX  __UINT8_MAX__
#define U16_MAX __UINT16_MAX__
#define U32_MAX __UINT32_MAX__
#define U64_MAX __UINT64_MAX__

typedef __INT8_TYPE__  i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

#define I8_MAX  __INT8_MAX__
#define I16_MAX __INT16_MAX__
#define I32_MAX __INT32_MAX__
#define I64_MAX __INT64_MAX__

typedef __UINTPTR_TYPE__ usize;
typedef __PTRDIFF_TYPE__ idiff;
#define USIZE_MAX __UINTPTR_MAX__
#define IDIFF_MAX __PTRDIFF_MAX__

typedef float f32;
typedef double f64;

typedef u8 Bool;
#define FALSE 0
#define TRUE 1

typedef const char * cstr;

#endif // INCLUDE_LIBK_TYPES_H_
