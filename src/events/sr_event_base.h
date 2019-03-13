#ifndef SR_EVENT_BASE_H
#define SR_EVENT_BASE_H

/*An evetn must have a type specified by its enum*/
template<typename EVENT_TYPE_TYPE> class SrEvent {
public:
    EVENT_TYPE_TYPE eventType;

    SrEvent(EVENT_TYPE_TYPE eventTypeArg) {
        eventType = eventTypeArg;
    }
};
#endif
