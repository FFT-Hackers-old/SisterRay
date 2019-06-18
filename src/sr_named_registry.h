#ifndef FUNCTION_REGISTRY_TEMPLATE_H
#define FUNCTION_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T, class S> class SrNamedResourceRegistry: public SrResourceRegistry<T> { //Publicly inherited, but should be private after refactor/cleanup
public:
    std::unordered_map<S, u32> named_registry;
    SrNamedResourceRegistry(): SrResourceRegistry() {};
    SrNamedResourceRegistry(SrKernelStream* stream) {
        size_t read_size;
        T object;

        /*Here we read from the KernelStream, and create objects with names ff7Base+idx*/
        u16 idx = 0;
        for (;;) {
            read_size = srKernelStreamRead(stream, &object, sizeof(object));
            if (read_size != sizeof(object))
                break;
            auto name = std::string(BASE_PREFIX) + std::to_string(idx);
            add_element(name, object);
            idx++;
        }
    }

    void add_element(const std::string& name, const T& element) {
        if (contains(name))
            return;
        add_resource(element);
        u32 index = resource_count() - 1;
        named_registry[name] = index;
    }

    void update_element(const std::string& name, const T& element) {
        if (!contains(name))
            return;
        update_resource(named_registry[name], element);
    }

    const T& get_element(const std::string& name) const {
        return get_resource(named_registry[name]);
    }

    T& get_element(const std::string& name) {
        return get_resource(named_registry[name]);
    }

    u32 get_resource_index(const std::string& name) {
        return named_registry[name];
    }

    bool contains(const std::string& name) {
        auto doesContain = (named_registry.find(name) == named_registry.end()) ? false : true;
        return doesContain;
    }
};

#endif
