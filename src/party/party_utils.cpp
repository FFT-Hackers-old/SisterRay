#include "party_utils.h"

u16 getGearEquipped(u8 characterID, u8 gearType) {
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u16 kernelObjectID;

    switch (gearType) { //probably refactor this into a utility
    case 0: {
        kernelObjectID = characterRecordArray[characterID].equipped_weapon;
        break;
    }
    case 1: {
        kernelObjectID = characterRecordArray[characterID].equipped_armor;
        break;
    }
    case 2: {
        kernelObjectID = characterRecordArray[characterID].equipped_weapon;
        break;
    }
    default: {
        return 0;
    }
    }
    return kernelObjectID;
}
