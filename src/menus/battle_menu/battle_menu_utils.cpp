#include "battle_menu_utils.h"
#include "../../impl.h"
#include "../../inventories/inventory_utils.h"

/*Return True if the character is usable by the character attempting to use it*/
bool isUsableInBattle(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.itemOnUseData.get_resource(itemID).characterRestrictionMask;
    bool itemIsUsuable = (bool)(!(restrictionMask & 2));

    auto party_member_index = (*BATTLE_ACTIVE_ACTOR_ID);
    auto characterID = (PARTY_STRUCT_ARRAY)[party_member_index].characterID;
    bool characterCanUse = (bool)!(characterMask & (1 << characterID));

    return (itemIsUsuable && characterCanUse);
}

bool isThrowable(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.itemOnUseData.get_resource(itemID).characterRestrictionMask;
    bool itemIsThrowable = (bool)(!(restrictionMask & 8)); // Use new throwable variable

    auto party_member_index = (*BATTLE_ACTIVE_ACTOR_ID);
    auto characterID = (PARTY_STRUCT_ARRAY)[party_member_index].characterID;
    bool characterCanUse = bool(characterMask & (1 << characterID));

    return (itemIsThrowable && characterCanUse);
}

bool didItemUseSucceed(u16 itemID) {
    bool isItemUsable;
    bool emptyItem = (itemID == 0xFFFF);

    if (emptyItem)
        return false;

    if (*COMMAND_TRIGGER_INDEX != 3 && *COMMAND_TRIGGER_INDEX != 10)
        isItemUsable = isThrowable(itemID);
    else
        isItemUsable = isUsableInBattle(itemID);

    return isItemUsable;
}

//Set the restore type global used by executing healing actions
u16 getRestoreTypeGlobal(i16 itemID) {
    auto itemType = gContext.itemTypeData.get_resource(itemID).item_type;
    if (itemType != 0) {
        return 0xFFFF;
    }
    return gContext.items.get_resource(itemID).resource_conditions;
}

// states should always be 0 (inactive) 1 (prepare on next update) 2 (active) or 3 (executing)
i32 setHandlerState(u16 handlerIndex, i8 state) {
    (HANDLER_STATE_ARRAY)[handlerIndex] = state;
    return handlerIndex;
}
