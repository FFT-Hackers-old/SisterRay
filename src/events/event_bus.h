#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <vector>
#include <unordered_map>
#include "non_copyable.h"
#include "event.h"
#include <unordered_map>

class EventBus : private NonCopyable
{
public:
    EventBus();
    ~EventBus();

    void dispatch(
        SrEventType eventType,
        void* event,
        const std::vector<SrEventContext>& contexts = std::vector<SrEventContext>()
    );
    SrEventListener addListener(
        SrEventType eventType,
        SrEventCallback callback,
        const std::string& modName,
        const std::unordered_set<SrEventContext>& keys = std::unordered_set<SrEventContext>()
    );

    void removeListener(SrEventListener listener);
    void addKey(SrEventListener listener, SrEventContext key);
    void removeKey(SrEventListener listener, SrEventContext key);

    //typesafe public helpers to make our type erasure safe

private:
    size_t _listenerCount;
    size_t _listenerCapacity;
    std::vector<SrEventType> _listenerTypes;
    std::vector<SrEventCallback> _listenerCallbacks;
    std::vector<std::vector<size_t>> _listenerRegistry; //vector of vectors containing listeners, i.e indexes into the listnerCallbacks/listenerTypes vectors
    std::unordered_map<std::string, std::vector<size_t>> _modListeners; //m
    std::vector<std::string> _listenerModNames;
    std::vector<std::unordered_set<SrEventContext>> _listenerContexts;
    std::vector<size_t> _freeListeners;
};

#endif
