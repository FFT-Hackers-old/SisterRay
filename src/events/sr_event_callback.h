#ifndef SR_EVENT_CALLBACK_H
#define SR_EVENT_CALLBACK_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_event_base.h"
#include <functional>

//Can be supplied an event as well as a set of Callback arguments
template<class EVENT_TYPE_TYPE, class LISTENER_TYPE> class SrEventCallback<EVENT_TYPE_TYPE> {
public:
    std::function<void(T, S)> handler;

    SrEventCallback(std::function<void(EVENT_TYPE_TYPE, LISTENER_TYPE > callback) {
        handler = callback;
    }
    void callHandler(EVENT_TYPE_TYPE event, LISTENER_TYPE callbackArgs) {
        handler(event, callbackArgs);
    }
}

#endif 
