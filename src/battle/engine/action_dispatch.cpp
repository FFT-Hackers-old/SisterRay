#include "action_dispatch.h"
#include "action_interface.h"


// Calling this function dispatches a battle action you have chosen to the action queue
void dispatchChosenBattleAction() {
    auto actorID = *BATTLE_ACTIVE_ACTOR_ID;
    auto battleType = (u8*)0x9A8762;
    if (*ISSUED_COMMAND_ID == CMD_MANIPULATE) {
        actorID = *ENEMY_ACTOR_IDX + 4;
    }

    auto targetingType = getChosenActionTargetType();
    auto actionID = getChosenActionID();
    auto actionTargetMask = getChosenActionValidTargetMask();
    auto actionTargetIndex = getChosenActionTargetIndex();
    auto actionRow = getChosenActionTargetRow();

    dispatchBattleAction(targetingType, actorID, *ISSUED_COMMAND_ID, actionID, actionTargetMask, actionTargetIndex, actionRow);
}

void dispatchFirstWAction() {
    auto actorID = *BATTLE_ACTIVE_ACTOR_ID;
    auto battleType = (u8*)0x9A8762;
    if (*ISSUED_COMMAND_ID == CMD_MANIPULATE) {
        actorID = *ENEMY_ACTOR_IDX + 4;
    }

    auto targetingType = *W_FIRST_TARGETING_CURSOR_TYPE;
    auto actionID = *W_FIRST_ACTION_USED;
    auto actionTargetMask = *W_FIRST_VALID_TARGET_MASK;
    auto actionTargetIndex = *W_FIRST_TARGET_INDEX;
    auto actionRow = *W_FIRST_STARTING_ROW;
    dispatchBattleAction(targetingType, actorID, *ISSUED_COMMAND_ID, actionID, actionTargetMask, actionTargetIndex, actionRow);
}

void dispatchBattleAction(u8 targetingType, u16 actorID, u16 commandID, u16 actionID, u16 targetMask, u16 targetIndex, u8 actionRow) {
    auto battleType = (u8*)0x9A8762;
    switch (targetingType) {
        case 0:
        case 1:
        case 2: {
            issueChosenAction(actorID, commandID, actionID, 1 << targetIndex);
            break;
        }
        case 4: {
            issueChosenAction(actorID, commandID, actionID, targetMask);
            break;
        }
        case 5: {
            if (*battleType != 3 && (*battleType <= 4u || *battleType > 7u))
                issueChosenAction(actorID, commandID, actionID, targetMask & 0xF);
            else
                issueChosenAction(actorID, commandID, actionID, BATTLE_ROW_MASKS[actionRow] & targetMask & 0xF);
            break;
        }
        case 6: {
            if (*battleType == 4)
                issueChosenAction(actorID, commandID, actionID, BATTLE_ROW_MASKS[actionRow] & targetMask & 0xFFF0);
            else
                issueChosenAction(actorID, commandID, actionID, targetMask & 0xFFF0);
            break;
        }
        default:
            return;
    }
}
