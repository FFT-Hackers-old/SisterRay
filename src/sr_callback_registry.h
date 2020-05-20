#ifndef SR_CALLBACK_REGISTRY_TEMPLATE_H
#define SR_CALLBACK_REGISTRY_TEMPLATE_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "kernel.h"
#include <unordered_map>
#include "sr_registry_template.h"


/*Maps item/resource indexes to callbacks defining their special effects*/
template<class T> class SrIndexedCallbackRegistry : public SrResourceRegistry<std::string> {
public:
    std::unordered_map<std::string, T> handler_names;
    SrIndexedCallbackRegistry() : SrResourceRegistry() {};

    void add_function(const std::string& name, const T& callback) {
        handler_names[name] = callback;
    }

    const T& get_handler(u16 materiaID) const {
        auto& name = getResource(materiaID);
        return handler_names[name];
    }

    T& get_handler(u16 materiaID) {
        auto& name = getResource(materiaID);
        return handler_names[name];
    }
};
#endif
