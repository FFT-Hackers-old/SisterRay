#ifndef SR_EVENT_BASE_H
#define SR_EVENT_BASE_H

/*An evetn must have a type specified by its enum*/
template<class EVENT_TYPE_TYPE> class SrEvent {
    EVENT_TYPE_TYPE eventType;
};
#endif
