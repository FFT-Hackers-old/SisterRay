#ifndef FUNCTION_REGISTRY_TEMPLATE_H
#define FUNCTION_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T, class S> class SrNamedResourceRegistry: public SrResourceRegistry<T> {
public:
    std::unordered_map<S, u32> named_registry;
    SrNamedResourceRegistry(): SrResourceRegistry() {};
    SrNamedResourceRegistry(SrKernelStream* stream) : SrResourceRegistry(stream) {};

    void add_element(const std::string& name, const T& element) {
        add_resource(element);
        u32 index = resource_count() - 1;
        named_registry[name] = index;
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
