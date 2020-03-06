#include "damage_callback_utils.h"
#include "status_names.h"
#include "../impl.h"

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic) {
    bool isDamageImmune = false;
    if (isMagic) {
        if (battleVars.stateFlags & 0x200)
            isDamageImmune = 1;
    }
    else if (battleVars.stateFlags & 0x100) {
        isDamageImmune = 1;
    }
    bool immuneStatii = (actorPosessesStatus(battleVars, srActorState, StatusNames::SHIELD) || actorPosessesStatus(battleVars, srActorState, StatusNames::PETRIFY));
    return isDamageImmune || immuneStatii;
}


bool actorPosessesStatus(const ActorBattleVars& battleVars, const ActorBattleState& srActorState, std::string statusName) {
    const auto& status = gContext.statuses.getElement(statusName);
    if (status.isGameStatus) {
        bool gameStatusActive = battleVars.statusMask & 1 << status.gameIndex;
        bool srStatusActive = std::find_if(srActorState.activeStatuses->begin(), srActorState.activeStatuses->end(),
            [&](ActiveStatus status){return status.statusName == statusName; }) != srActorState.activeStatuses->end();
        if (srStatusActive != gameStatusActive) {
            srLogWrite("sr status and game status are not synced for status %s", statusName.c_str());
        }
        return gameStatusActive && srStatusActive;
    }
    else {
        return std::find_if(srActorState.activeStatuses->begin(), srActorState.activeStatuses->end(),
            [&](ActiveStatus status) {return status.statusName == statusName; }) != srActorState.activeStatuses->end();
    }
}
