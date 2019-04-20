#include <algorithm>
#include "event_bus.h"
#include <unordered_set>

EventBus::EventBus(): _listenerCount(0), _listenerCapacity(1) {
    _listenerTypes.push_back(NO_TYPE);
    _listenerCallbacks.push_back(nullptr);
}

EventBus::~EventBus() {
}

void EventBus::dispatch(SrEventType eventType, void* event, const std::vector<SrEventContext>& contexts) {
    if (!eventType)
        return;

    if (_listenerRegistry.size() <= eventType)
        return;

    const std::vector<size_t>& eventListeners = _listenerRegistry[eventType];

    for (size_t listenerID : eventListeners) {
        const auto& keys = _listenerContexts[listenerID];
        if ((!keys.empty()) && (!contexts.empty())) {
            for (auto context : contexts) {
                if (keys.find(context) == keys.end())
                    return;
            }
        }
        SrEventCallback callback = _listenerCallbacks[listenerID];
        callback(event);
    }
}

SrEventListener EventBus::addListener(SrEventType eventType, SrEventCallback callback, const std::string& modName, const std::unordered_set<SrEventContext>& keys) {
    size_t listenerID;

    if (!eventType)
        return 0;

    if (_listenerRegistry.size() <= eventType)
        _listenerRegistry.resize((size_t)eventType + 1);

    std::vector<size_t>& eventListeners = _listenerRegistry[eventType];

    if (!_freeListeners.empty()) {
        listenerID = _freeListeners.back();
        _freeListeners.pop_back();
    }
    else {
        listenerID = _listenerCapacity++;
        _listenerCallbacks.resize(listenerID + 1);
        _listenerTypes.resize(listenerID + 1);
        _listenerContext.resize(listenerID + 1);
    }

    SrEventListener listener = { listenerID, keys };
    _modListeners[modName].push_back(listenerID);
    _listenerModNames[listenerID] = modName;
    _listenerCallbacks[listenerID] = callback;
    _listenerTypes[listenerID] = eventType;
    _listenerCount++;
    eventListeners.push_back(listenerID);
    _listenerContexts[listenerID] = keys;

    return listener;
}

void EventBus::addKey(SrEventListener listener, u32 key) {
    if (!listenerID)
        return;

    if (listenerID >= _listenerCapacity)
        return;

    _listenerContexts[listenerID].insert(key);
}

void EventBus::removeKey(SrEventListener listener, u32 key) {
    if (!listenerID)
        return;

    if (listenerID >= _listenerCapacity)
        return;

    _listenerContexts[listenerID].erase(key);
}

void EventBus::removeListener(SrEventListener listener) {
    auto listenerID = listener.listenerID;
    if (!listenerID)
        return;

    if (listenerID >= _listenerCapacity)
        return;

    SrEventType eventType = _listenerTypes[listenerID];
    if (!eventType)
        return;

    std::vector<size_t>& eventListeners = _listenerRegistry[eventType];
    std::remove(eventListeners.begin(), eventListeners.end(), listenerID);
    _listenerTypes[listenerID] = NO_TYPE;
    _listenerCallbacks[listenerID] = nullptr;

    const auto& modName = _listenerModNames[listenerID];
    std::vector<size_t>& modListeners = _modListeners[modName];
    std::remove(modListeners.begin(), modListeners.end(), listenerID);
    _listenerModNames[listenerID] = std::string("FREE");

    _listenerContexts[listenerID].empty();

    _freeListeners.push_back(listenerID);
    _listenerCount--;
}
