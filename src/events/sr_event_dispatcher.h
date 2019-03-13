#ifndef SR_EVENT_DiSPATCHER_H
#define SR_EVENT_DISPATCHER_H

#include <unordered_map>
#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_event_base.h"

/*maintain registered event-listeners which will be triggered when a particular event is dispatched
 One of these will be constructed for each "menu" pane. T must be the eventTYpe of the event W*/
template<class EVENT_TYPE_TYPE, class LISTENER_TYPE, class EVENT_TYPE, typename DISPATCH_ARGS_TYPE> class SrEventDispatcher {
public:
    std::unordered_map<EVENT_TYPE_TYPE, std::vector<LISTENER_TYPE>> eventCallbacks;

    void addListener(EVENT_TYPE_TYPE eventType, LISTENER_TYPE eventListener) {
        eventCallbacks[eventType].push_back(eventListener);
    }

    void removeListener(EVENT_TYPE_TYPE eventType, u32 index) {
        eventCallbacks[eventType].erase(eventCallbacks[eventType].begin() + index);
    }

    /*Invoke every callback registered to the eventType of the event*/
    void dispatchEvent(EVENT_TYPE event, DISPATCH_ARGS_TYPE eventArgs) {
        auto eventType = event.eventType;
        for (auto callback = eventCallbacks[eventType]; callback != eventCallbacks[eventType].end(); callback++) {
            (*callback)(event, eventArgs);
        }
    }
}
#endif 
