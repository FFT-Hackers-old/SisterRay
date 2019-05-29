#include "party_callbacks.h"
#include "../impl.h"

void registerPartyCallbacks() {
    const auto& modName = std::string("srFF7Base");
    std::unordered_set<SrEventContext> dispatchContextT6S1 = { MAT_HIGH_TYPE_6, MAT_SUB_TYPE_1 };
    gContext.eventBus.addListener(ENABLE_ACTIONS, (SrEventCallback)enableAblT6S1, modName, dispatchContextT6S1);
    gContext.eventBus.addListener(APPLY_SUPPORT, (SrEventCallback)counterT6S1, modName, dispatchContextT6S1);

    std::unordered_set<SrEventContext> dispatchContextT9S1 = { MAT_HIGH_TYPE_9, MAT_SUB_TYPE_1 };
    gContext.eventBus.addListener(ENABLE_ACTIONS, (SrEventCallback)enableAblT9S1, modName, dispatchContextT9S1);
    gContext.eventBus.addListener(APPLY_SUPPORT, (SrEventCallback)mCounterT9S1, modName, dispatchContextT9S1);
}
