#include "party_utils.h"
#include "../inventories/inventory_utils.h"

u16 getEquippedGear(u8 characterID, u8 gearType) {
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u16 kernelObjectID;

    switch (gearType) { //probably refactor this into a utility
    case 1: {
        kernelObjectID = characterRecordArray[characterID].equipped_weapon;
        break;
    }
    case 2: {
        kernelObjectID = characterRecordArray[characterID].equipped_armor;
        break;
    }
    case 3: {
        kernelObjectID = characterRecordArray[characterID].equipped_weapon;
        break;
    }
    default: {
        return 0;
    }
    }
    return kernelObjectID;
}

bool characterCanEquipItem(u8 characterID, u16 item_id){
    auto characterMask = getCharacterRestrictionMask(item_id);
    bool characterCanUse = (bool)(characterMask & (1 << characterID));

    return characterCanUse;
}



