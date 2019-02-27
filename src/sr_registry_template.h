#ifndef REGISTRY_TEMPLATE_H
#define REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <vector>

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T> class SrResourceRegistry {
public:
    /*Constructor for initializing a resource registry from a kernel stream*/
    SrResourceRegistry(SrKernelStream* stream) {
        size_t read_size;
        T object;

        for (;;)
        {
            read_size = srKernelStreamRead(stream, &object, sizeof(object));
            if (read_size != sizeof(object))
                break;
            SrResourceRegistry::add_resource(object);
        }
    }

    /*Constructor used for fixed size arrays, like the inventory*/
    SrResourceRegistry(int reserve_size) {
        SrResourceRegistry::resource_registry.reserve(reserve_size);
    }

    /*Default constructor*/
    SrResourceRegistry() {}
    ~SrResourceRegistry() {}

    T get_resource(int index) {
        if ((SrResourceRegistry::resource_count() == 0)||(index >= (SrResourceRegistry::resource_count() - 1))) {
            return T();
        }
        return SrResourceRegistry::resource_registry[index];
    }

    void add_resource(T resource) {
        SrResourceRegistry::resource_registry.push_back(resource);
    }

    int resource_count() {
        return resource_registry.size();
    }

    T* get_data() {
        return resource_registry.data();
    }

    std::vector<T> resource_registry;
};

#endif
