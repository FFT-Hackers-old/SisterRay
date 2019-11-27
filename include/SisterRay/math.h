#ifndef MATH_H
#define MATH_H

#include "types.h"

typedef struct {
    float x;
    float y;
    float z;
} R3Point;

typedef struct {
    float elements[4][4];
} Matrix;
#endif // !MATH_H
