#ifndef MATERIA_H
#define MATERIA_H

#include <SisterRay/types.h>

#pragma pack(push, 1)
typedef struct {
    u16     apLevel[4];
    u8      equipEffect;
    u32     statusEffect;
    u8      type;
    u8      data[6];
} MateriaData;
#pragma pack(pop)

typedef struct
{
    size_t          count;
    size_t          capacity;
    MateriaData*    data;
} SrMateriaRegistry;

#endif
