#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <functional>
#include "impl.h"

/*Allocate space for an initial 16 objects*/
template<typename T, typename S> void initObjectRegistry(S* registry, int capacity = 16) {
    registry->capacity = capacity;
    registry->count = 0;
    registry->data = (T*)malloc(registry->capacity * sizeof(*registry->data));
}

/*Allocate space for an inventory-type object*/
template<typename T, typename S> void initStaticObjectRegistry(S* registry, int capacity = 16) {
    registry->capacity = capacity;
    registry->data = (T*)malloc(registry->capacity * sizeof(*registry->data));
}

/*Allocate space in the armor registry for a new armor, increasing the array size if necessary*/
template<typename T, typename S> T* allocKernelObject(S* registry) {
    size_t newCapacity;
    T* object;

    if (registry->capacity == registry->count)
    {
        newCapacity = registry->capacity + registry->capacity / 2;
        registry->data = (T*)realloc(registry->data, newCapacity * sizeof(*registry->data));
        registry->capacity = newCapacity;
    }

    object = registry->data + registry->count;
    registry->count++;
    memset(object, 0, sizeof(*object));

    return object;
}

/*template function to initialize the registry for a given type of kernel object*/
template<typename T, typename S, typename F, typename G> void initRegistry(SrKernelStream* stream, S* registry, F allocator, G initializer) {
    size_t ret;
    T tmp;
    T* object;
    initializer(registry, 16);

    for (;;)
    {
        ret = srKernelStreamRead(stream, &tmp, sizeof(tmp));
        if (ret != sizeof(tmp))
            break;
        object = allocator(registry);
        memcpy(object, &tmp, sizeof(tmp));
    }
}
#endif
