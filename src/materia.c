#include <stdlib.h>
#include <string.h>
#include "impl.h"

static void InitMateriaRegistry(void)
{
    SrMateriaRegistry* reg = &gContext.materias;
    reg->capacity = 16;
    reg->count = 0;
    reg->data = malloc(reg->capacity * sizeof(*reg->data));
}

static MateriaData* AllocMateria(void)
{
    size_t newCapacity;
    SrMateriaRegistry* reg = &gContext.materias;
    MateriaData* materia;

    if (reg->capacity == reg->count)
    {
        newCapacity = reg->capacity + reg->capacity / 2;
        reg->data = realloc(reg->data, newCapacity * sizeof(*reg->data));
        reg->capacity = newCapacity;
    }
    materia = reg->data + reg->count;
    reg->count++;
    memset(materia, 0, sizeof(*materia));

    return materia;
}

SISTERRAY_API void InitMateria(SrKernelStream* stream)
{
    size_t ret;
    MateriaData tmp;
    MateriaData* materia;
    InitMateriaRegistry();

    for (;;)
    {
        ret = srKernelStreamRead(stream, &tmp, sizeof(tmp));
        if (ret != sizeof(tmp))
            break;
        materia = AllocMateria();
        memcpy(materia, &tmp, sizeof(tmp));
    }
    srLogWrite("kernel.bin: Loaded %lu Materias", (unsigned long)gContext.materias.count);
}
