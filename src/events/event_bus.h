#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <vector>
#include <unordered_map>
#include "non_copyable.h"
#include "event.h"

class EventBus : private NonCopyable
{
public:
    EventBus();
    ~EventBus();

    void dispatch(SrEventType type, const void* event);
    SrEventListener addListener(SrEventType eventType, SrEventCallback callback);
    void removeListener(SrEventListener listener);

    //typesafe public helpers to make our type erasure safe

private:
    size_t _listenerCount;
    size_t _listenerCapacity;
    std::vector<SrEventType> _listenerTypes;
    std::vector<SrEventCallback> _listenerCallbacks;
    std::vector<std::vector<size_t>> _listenerRegistry; //vector of vectors containing listeners, i.e indexes into the listnerCallbacks/listenerTypes vectors
    std::vector<size_t> _freeListeners;
};

#endif
