#ifndef EVENT_BUS_INTERFACE_H
#define EVENT_BUS_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API u32 srAddListener(SrEventType eventType, SrEventCallback callback, const char* modName, SrEventContext context = INVALID_CONTEXT);
SISTERRAY_API u32 dispatchEvent(SrEventType eventTYpe, void* event);
#endif // !EVENT_BUS_INTERFACE_H
