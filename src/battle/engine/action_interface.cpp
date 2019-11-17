#include "action_interface.h"

void setChosenCommandID(u8 commandID) {
    *ISSUED_COMMAND_ID = commandID;
}

u8 getChosenCommandID() {
    return *ISSUED_COMMAND_ID;
}

void setChosenActionID(u16 actionID) {
    *ISSUED_ACTION_ID = actionID;
}

u16 getChosenActionID() {
    return *ISSUED_ACTION_ID;
}

void setChosenActionMenuIndex(u16 menuIndex) {
    *ISSUED_ACTION_MENU_INDEX = menuIndex;
}

u16 getChosenActionMenuIndex() {
    return *ISSUED_ACTION_MENU_INDEX;
}

void setChosenActionTargetData(u8 targetFlags) {
    *ISSUED_ACTION_TARGET_DATA = targetFlags;
}

u8 getChosenActionTargetData() {
    return *ISSUED_ACTION_TARGET_DATA;
}

void setChosenActionTargetType(u8 targetType) {
    *ISSUED_ACTION_TARGET_TYPE = targetType;
}

u8 getChosenActionTargetType(){
    return *ISSUED_ACTION_TARGET_TYPE;
}

void setChosenActionTargetIndex(u8 targetIndex) {
    *ISSUED_ACTION_TARGET_INDEX = targetIndex;
}

u8 getChosenActionTargetIndex() {
    return *ISSUED_ACTION_TARGET_INDEX;
}

void setChosenActionTargetRow(u8 targetRow) {
    *ISSUED_ACTION_STARTING_ROW = targetRow;
}

u8 getChosenActionTargetRow() {
    return *ISSUED_ACTION_STARTING_ROW;
}

void setChosenActionValidTargetMask(u16 targetMask) {
    *ISSUED_ACTION_VALID_TARGET_MASK = targetMask;
}

u8 getChosenActionValidTargetMask() {
    return *ISSUED_ACTION_VALID_TARGET_MASK;
}
