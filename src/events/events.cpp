#include "event_bus.h"
#include "../api.h"
#include "../impl.h"
#include <stdarg.h>

SISTERRAY_API SrEventListener srAddListener(SrEventType eventType, SrEventCallback callback, char* modName, u32 context) {
    std::unordered_set<u32>& keys({ context });
    return gContext.eventBus.addListener(eventType, callback, std::string(modName), keys);
}
