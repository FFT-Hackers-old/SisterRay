#include "cmd_select_callbacks.h"
#include "../battle/battle_engine_interface.h"
#include "../impl.h"

void handleCommandTarget(SelectCommandEvent* event) {
    u8* byte_DC3C8C = (u8*)0xDC3C8C;
    *byte_DC3C8C = 1;
    setMenuState(event->menuObect, BATTLE_TARGETING_STATE);
    setTargetingFromFlags(event->command->targetingData, false);
}

void handleWeaponTarget(SelectCommandEvent* event) {
    u8* byte_DC3C8C = (u8*)0xDC3C8C;
    *byte_DC3C8C = 0;
    srLogWrite("setting command targeting to: %x", event->command->targetingData);
    setMenuState(event->menuObect, BATTLE_TARGETING_STATE);
    setTargetingFromFlags(event->command->targetingData, false);
}

void cmdMagicSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_MAGIC_STATE);
}

void cmdSummonSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_SUMMON_STATE);
}

void cmdItemSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_ITEM_STATE);
}

void cmdLimitSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_LIMIT_STATE);
}

void cmdWMagicSelectHandler(SelectCommandEvent* event) {
    *W_COMMAND_ENABLED = 1;
    cmdMagicSelectHandler(event);
}

void cmdWSummonSelectHandler(SelectCommandEvent* event) {
    *W_COMMAND_ENABLED = 1;
    cmdSummonSelectHandler(event);
}

void cmdWItemSelectHandler(SelectCommandEvent* event) {
    *W_COMMAND_ENABLED = 1;
    cmdItemSelectHandler(event);
}
