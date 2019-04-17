#ifndef REGISTRY_TEMPLATE_H
#define REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <vector>
#include <memory>

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
            add_resource(object);
        }
    }

    /*Constructor used for fixed size arrays, like the inventory*/
    SrResourceRegistry(u32 reserve_size) {
        resource_registry.reserve(reserve_size);
    }

    /*Default constructor*/
    SrResourceRegistry() {
    }
    ~SrResourceRegistry() {
    }

    T get_resource(i32 index) {
        if ((resource_count() == 0)||(index > (resource_count() - 1))) {
            return T();
        }
        return resource_registry[index];
    }

    T& get_resource_ref(i32 index) {
        return resource_registry.at(index);
    }

    void add_resource(T resource) {
        resource_registry.push_back(resource);
    }

    void update_resource(u32 index, T resource) {
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


    T* get_data() {
        return resource_registry.data();
    }

    std::vector<T> resource_registry;
};

#endif
