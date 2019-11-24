#ifndef SISTER_RAY_TYPES_H
#define SISTER_RAY_TYPES_H

#include <stdint.h>

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

#pragma pack(push, 1)
struct i24 {
    unsigned int attackData : 24;
};
#pragma pack(pop)

#endif
