#ifndef FUNCTION_REGISTRY_TEMPLATE_H
#define FUNCTION_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"
#include "gamedata/gdata_utils.h"

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
            auto name = assembleGDataKey(idx);
            addElement(name, object);
            idx++;
        }
    }

    void addElement(const std::string& name, const T& element) {
        if (contains(name))
            return;
        addResource(element);
        u32 index = resourceCount() - 1;
        named_registry[name] = index;
    }

    void updateElement(const std::string& name, const T& element) {
        if (!contains(name))
            return;
        updateResource(named_registry[name], element);
    }

    const T& getElement(const std::string& name) const {
        return getResource(named_registry[name]);
    }

    T& getElement(const std::string& name) {
        return getResource(named_registry[name]);
    }

    u32 getResourceIndex(const std::string& name) {
        if (contains(name)) {
            return named_registry[name];
        }
        return 0;
    }

    bool contains(const std::string& name) {
        auto doesContain = (named_registry.find(name) == named_registry.end()) ? false : true;
        return doesContain;
    }
};

#endif
