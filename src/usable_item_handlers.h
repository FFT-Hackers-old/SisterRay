#ifndef USABLE_ITEM_HANDLERS_H
#define USABLE_ITEM_HANDLERS_H

#include <SisterRay/types.h>
#include "sr_registry_template.h"
#include <unordered_map>

typedef bool(*onUseCallback)(u16, u16, u16);

/*Hold the names of functions along with function ptrs*/

class srOnUseCallbackRegistry : public SrResourceRegistry<std::string> {
public:
    std::unordered_map<std::string, onUseCallback> handler_names;
    srOnUseCallbackRegistry() : SrResourceRegistry<std::string>() {}

    void add_function(std::string name, onUseCallback callback);
    onUseCallback get_handler(u16 item_id);
};

#endif
