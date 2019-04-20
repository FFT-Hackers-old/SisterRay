#include "event_bus.h"
#include "../api.h"
#include "../impl.h"
#include <stdarg.h>

SISTERRAY_API SrEventListener srAddListener(SrEventType eventType, SrEventCallback callback, const char* modName, SrEventContext context) {
    const std::unordered_set<SrEventContext>& keys({ context });
    return gContext.eventBus.addListener(eventType, callback, std::string(modName), keys);
}
