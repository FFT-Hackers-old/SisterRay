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
    i16 x;
    i16 y;
    i16 z;
} R3PointWord;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    i32 x;
    i32 y;
    i32 z;
} R3PointDWord;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    float elements[4][4];
} Matrix;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    i16 R3Submatrix[3][3];
    i32 position[3];
} GameRotationMatrix;
#pragma pack(pop)

#endif // !MATH_H
