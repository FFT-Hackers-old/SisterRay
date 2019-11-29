#ifndef MATH_H
#define MATH_H

#include "types.h"

#pragma pack(push, 1)
typedef struct {
    float x;
    float y;
    float z;
} R3Point;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    float elements[4][4];
} Matrix;
#pragma pack(pop)

#endif // !MATH_H
