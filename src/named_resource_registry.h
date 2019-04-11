#ifndef FUNCTION_REGISTRY_TEMPLATE_H
#define FUNCTION_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T> class SrNamedResourceRegistry: public SrResourceRegistry<std::string> {
public:
    std::unordered_map<std::string, T> named_registry;
    SrNamedResourceRegistry(): SrResourceRegistry() {};

    void add_element(std::string name, T element) {
        named_registry[name] = element;
    }

    T get_element(std::string name) {
        return named_registry[name];
    }

    T& getElementRef(std::string name) {
        return named_registry.at(name);
    }

    T get_handler(u16 item_id) {
        auto& name = get_resource(item_id);
        return named_registry[name];
    }
};

#endif
