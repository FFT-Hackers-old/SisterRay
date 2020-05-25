#include "event_bus.h"
#include "../api.h"
#include "../impl.h"
#include <stdarg.h>

SISTERRAY_API u32 srAddListener(SrEventType eventType, SrEventCallback callback, const char* modName, SrEventContext context) {
    const std::unordered_set<SrEventContext>& keys({ context });
    SrEventListener listener;
    if (context == INVALID_CONTEXT) {
        listener = gContext.eventBus.addListener(eventType, callback, std::string(modName));
    }
    else {
        listener = gContext.eventBus.addListener(eventType, callback, std::string(modName), keys);
    }
   // srLogWrite("Registering Mod Callaback for mod %s to event %i", modName, eventType);
    return listener.listenerID;
}

SISTERRAY_API void dispatchEvent(SrEventType eventType, void* srEvent){
    gContext.eventBus.dispatch(eventType, srEvent);
}
