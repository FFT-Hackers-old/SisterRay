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

        for (;;) {
            read_size = srKernelStreamRead(stream, &object, sizeof(object));
            if (read_size != sizeof(object))
                break;
            add_resource(object);
        }
    }

    /*Constructor used for fixed size arrays, like the inventory*/
    SrResourceRegistry(u32 reserve_size) {
        _resource_registry.reserve(reserve_size);
    }

    /*Default constructor*/
    SrResourceRegistry() {}
    ~SrResourceRegistry() {}

    const T& get_resource(u32 index) const {
        if (index >= resource_count()) {
            return _null;
        }
        return _resource_registry[index];
    }

    T& get_resource(u32 index) {
        if (index >= resource_count()) {
            return _null;
        }
        return _resource_registry[index];
    }

    void add_resource(const T& resource) {
        _resource_registry.push_back(resource);
    }

    void update_resource(u32 index, const T& resource)
    {
        if (index < resource_count())
        {
            _resource_registry[index] = resource;
        }
    }

    size_t resource_count()
    {
        return _resource_registry.size();
    }

    size_t current_capacity()
    {
        return _resource_registry.capacity();
    }

    T* get_data()
    {
        return _resource_registry.data();
    }

protected:
    std::vector<T>  _resource_registry;

private:
    T               _null;
};

#endif
