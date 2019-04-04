#include <algorithm>
#include "event_bus.h"

EventBus::EventBus(): _listenerCount(0), _listenerCapacity(1) {
    _listenerTypes.push_back(NO_TYPE);
    _listenerCallbacks.push_back(nullptr);
}

EventBus::~EventBus() {
}

void EventBus::dispatch(SrEventType eventType, void* event) {
    if (!eventType)
        return;

    if (_listenerRegistry.size() <= eventType)
        return;

    const std::vector<size_t>& eventListeners = _listenerRegistry[eventType];

    for (size_t listenerID : eventListeners) {
        SrEventCallback callback = _listenerCallbacks[listenerID];
        callback(event);
    }
}

SrEventListener EventBus::addListener(SrEventType eventType, SrEventCallback callback) {
    size_t listenerID;

    if (!eventType)
        return 0;

    if (_listenerRegistry.size() <= eventType)
        _listenerRegistry.resize((size_t)eventType);

    std::vector<size_t>& eventListeners = _listenerRegistry[eventType];

    if (!_freeListeners.empty()) {
        listenerID = _freeListeners.back();
        _freeListeners.pop_back();
    }
    else {
        listenerID = _listenerCapacity++;
        _listenerCallbacks.resize(listenerID);
        _listenerTypes.resize(listenerID);
    }
    _listenerCallbacks[listenerID] = callback;
    _listenerTypes[listenerID] = eventType;
    _listenerCount++;
    eventListeners.push_back(listenerID);

    return SrEventListener(listenerID);
}

void EventBus::removeListener(SrEventListener listener) {
    if (!listener)
        return;

    if (listener >= _listenerCapacity)
        return;

    SrEventType eventType = _listenerTypes[listener];
    if (!eventType)
        return;

    std::vector<size_t>& eventListeners = _listenerRegistry[eventType];
    std::remove(eventListeners.begin(), eventListeners.end(), listener);
    _listenerTypes[listener] = NO_TYPE;
    _listenerCallbacks[listener] = nullptr;
    _freeListeners.push_back(listener);
    _listenerCount--;
}
