#include "event_bus.h"
#include "../api.h"
#include "../impl.h"

SISTERRAY_API SrEventListener srAddListener(SrEventType eventType, SrEventCallback callback)
{
    return gContext.eventBus.addListener(eventType, callback);
}
