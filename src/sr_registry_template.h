#ifndef REGISTRY_TEMPLATE_H
#define REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <vector>
#include <memory>

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class EVENT_TYPE_TYPE> class SrResourceRegistry {
public:
    /*Constructor for initializing a resource registry from a kernel stream*/
    SrResourceRegistry(SrKernelStream* stream) {
        size_t read_size;
        EVENT_TYPE_TYPE object;

        for (;;)
        {
            read_size = srKernelStreamRead(stream, &object, sizeof(object));
            if (read_size != sizeof(object))
                break;
            add_resource(object);
        }
    }

    /*Constructor used for fixed size arrays, like the inventory*/
    SrResourceRegistry(int reserve_size) {
        resource_registry.reserve(reserve_size);
        srLogWrite("ptr to class instance: %p", this);
        srLogWrite("ptr to std:vector underlying instance: %p", &(this->resource_registry));
        srLogWrite("initializing reserve registry with size: %lu", (unsigned long)resource_registry.capacity());
    }

    /*Default constructor*/
    SrResourceRegistry() {
    }
    ~SrResourceRegistry() {
    }

    EVENT_TYPE_TYPE get_resource(int index) {
        if ((resource_count() == 0)||(index >= (resource_count() - 1))) {
            return EVENT_TYPE_TYPE();
        }
        return resource_registry[index];
    }

    void add_resource(EVENT_TYPE_TYPE resource) {
        resource_registry.push_back(resource);
    }

    void update_resource(int index, EVENT_TYPE_TYPE resource) {
        if (index < (resource_count())){
            resource_registry[index] = resource;
        }
    }

    int resource_count() {
        return resource_registry.size();
    }

    int current_capacity() {
        return resource_registry.capacity();
    }


    EVENT_TYPE_TYPE* get_data() {
        return resource_registry.data();
    }

    std::vector<EVENT_TYPE_TYPE> resource_registry;
};

#endif
