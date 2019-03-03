#ifndef FUNCTION_REGISTRY_TEMPLATE_H
#define FUNCTION_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"

/*Simple C++ template to replace all the old school manually resized C arrays
  And unify our String Registry and all of our various item registries*/
template<class T> class SrFunctionResourceRegistry: public SrResourceRegistry<std::string> {
public:
    std::unordered_map<std::string, T> handler_names;
    SrFunctionResourceRegistry(): SrResourceRegistry() {};

    void add_function(std::string name, T callback) {
        handler_names[name] = callback;
    }

    T get_handler(u16 item_id) {
        auto& name = get_resource(item_id);
        return handler_names[name];
    }
};

#endif
