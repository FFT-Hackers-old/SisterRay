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
    u16 x;
    u16 y;
    u16 z;
} R3PointWord;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    float elements[4][4];
} Matrix;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    u16 R3Submatrix[3][3];
    u32 Colummn4[3];
} GameRotationMatrix;
#pragma pack(pop)

#endif // !MATH_H
