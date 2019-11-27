#include "action_targeting.h"
#include "action_interface.h"


void setTargetingFromFlags(u8 flags, u32 reverseRow) {
    setChosenActionTargetData(flags);
    setTargetingDataGlobals(reverseRow);
}

void setTargetingDataGlobals(u32 reverseRow) {
    u8* byte_DC3C98 = (u8*)0xDC3C98;
    setValidTargetMask();
    auto targetData = getChosenActionTargetData();
    if (targetData & TGT_FLAG_START_MULTIPLE) {
        if (targetData & TGT_FLAG_ALL_ROWS) {
            setChosenActionTargetType(4);
        }
        else {
            if (reverseRow) {
                if (targetData & TGT_FLAG_START_ENEMIES) {
                    setChosenActionTargetType(TGT_TYPE_ALLIES_ALL);
                    setChosenActionTargetIndex(0);
                }
                else {
                    setChosenActionTargetType(TGT_TYPE_ENEMIES_ALL);
                    setChosenActionTargetIndex(4);
                }
            }
            else {
                if (targetData & TGT_FLAG_START_ENEMIES) {
                    setChosenActionTargetType(TGT_TYPE_ENEMIES_ALL);
                    setChosenActionTargetIndex(4);
                }
                else {
                    setChosenActionTargetType(TGT_TYPE_ALLIES_ALL);
                    setChosenActionTargetIndex(*BATTLE_ACTIVE_ACTOR_ID);
                }
            }
            setChosenActionTargetRow(getTargetRow());
        }
    }
    else if (targetData & TGT_FLAG_START_ENEMIES) {
        if (reverseRow) {
            setChosenActionTargetType((targetData & TGT_FLAG_ONE_ROW) != 0);
            setChosenActionTargetIndex(0);
        }
        else {
            if (targetData & TGT_FLAG_ONE_ROW)
                setChosenActionTargetType(2);
            else
                setChosenActionTargetType(0);
            setChosenActionTargetIndex(4);
        }
    }
    else {
        if (reverseRow) {
            setChosenActionTargetType((targetData & TGT_FLAG_ONE_ROW) != 0);
            setChosenActionTargetIndex(*BATTLE_ACTIVE_ACTOR_ID);
        }
        else {
            if (targetData & TGT_FLAG_ONE_ROW)
                setChosenActionTargetType(2);
            else
                setChosenActionTargetType(0);
            setChosenActionTargetIndex(*ENEMY_ACTOR_IDX);
        }
    }

    if (!((1 << getChosenActionTargetIndex()) & getChosenActionValidTargetMask())) {
        sub_6E59BC(getChosenActionTargetType());
        setChosenActionTargetRow(getTargetRow());
    }
    *byte_DC3C98 = getChosenActionTargetIndex();
    *BATTLE_TARGETED_GLOBAL = getChosenActionTargetIndex();
}

u8 getTargetRow() {
    for (auto row = 0; row < 3; ++row) {
        if (BATTLE_ROW_MASKS[row] & ((1 << getChosenActionTargetIndex()) & getChosenActionValidTargetMask())) {
            return row;
        }
    }
    return 0;
}

void setValidTargetMask() {
    u8* byte_DC3C8C = (u8*)0xDC3C8C;
    u16* word_9A8898 = (u16*)0x9A8898;
    if (*byte_DC3C8C == 1) {
        setChosenActionValidTargetMask(*word_9A889A);
        return;
    }
    setChosenActionValidTargetMask(*word_9A8898);
}

void storeFirstWCommandIssued() {
    *W_FIRST_ACTION_USED = getChosenActionID();
    *W_FIRST_ACTION_INDEX = getChosenActionMenuIndex();
    *W_FIRST_TARGETING_CURSOR_TYPE = getChosenActionTargetType();
    *W_FIRST_VALID_TARGET_MASK = getChosenActionValidTargetMask();
    *W_FIRST_TARGET_INDEX = getChosenActionTargetIndex();
    *W_FIRST_STARTING_ROW = getChosenActionTargetType();
}
