#ifndef REGISTRY_TEMPLATE_H
#define REGISTRY_TEMPLATE_H

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

        /*Here we read from the KernelStream */
        for (;;) {
            read_size = srKernelStreamRead(stream, &object, sizeof(object));
            if (read_size != sizeof(object))
                break;
            addResource(object);
        }
    }

    /*Constructor used for fixed size arrays, like the inventory*/
    SrResourceRegistry(u32 reserve_size) {
        _resource_registry.reserve(reserve_size);
    }

    /*Default constructor*/
    SrResourceRegistry() {}
    ~SrResourceRegistry() {}

    const T& getResource(u32 index) const {
        if (index >= resourceCount()) {
            return _null;
        }
        return _resource_registry[index];
    }

    T& getResource(u32 index) {
        if (index >= resourceCount()) {
            return _null;
        }
        return _resource_registry[index];
    }

    void addResource(const T& resource) {
        _resource_registry.push_back(resource);
    }

    void updateResource(u32 index, const T& resource) {
        if (index < resourceCount()) {
            _resource_registry[index] = resource;
        }
    }

    void setResource(u32 index, const T& resource) {
        if (index > _resource_registry.size()) {
            _resource_registry.resize(index + 1);
        }
        _resource_registry[index] = resource;
    }

    size_t resourceCount() {
        return _resource_registry.size();
    }

    size_t currentCapacity() {
        return _resource_registry.capacity();
    }

    T* getData() {
        return _resource_registry.data();
    }

protected:
    std::vector<T>  _resource_registry;

private:
    T               _null;
};

#endif
