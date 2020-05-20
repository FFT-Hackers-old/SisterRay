#include "party_utils.h"
#include "../inventories/inventory_utils.h"
#include "../impl.h"

std::string getCharacterName (u8 characterID) {
    switch (characterID) {
        case 0:
            return std::string("CLOUD");
        case 1:
            return std::string("BARRET");
        case 2:
            return std::string("TIFA");
        case 3:
            return std::string("AERIS");
        case 4:
            return std::string("REDXIII");
        case 5:
            return std::string("YUFFIE");
        case 6:
            return std::string("CAITSITH");
        case 7:
            return std::string("VINCENT");
        case 8:
            return std::string("CID");
        case 9:
            return std::string("YCLOUD");
        case 10:
            return std::string("SEPHIROTH");
        default: {
            return std::string("NULL");
        }
    }
}

PartyMember* getGamePartyMember(u8 actorIdx) {
    if (G_SAVE_MAP->activeParty[actorIdx] == 0xFF) {
        return nullptr;
    }
    return &(PARTY_STRUCT_ARRAY[actorIdx]);
}

bool characterCanEquipItem(u8 characterID, u16 itemID) {
    return canCharacterUseItem(characterID, itemID);
}

u8 getCharacterRecordIndex(u8 characterIdx) {
    return (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[characterIdx])];
}

bool isPartyActor(u8 actorIdx) {
    return (actorIdx < 3);
}

bool actorHasStatus(u8 actorID, u32 maskOfStatus) {
    auto& actorAIStates = AI_BATTLE_CONTEXT->actorAIStates;
    return actorAIStates[actorID].statusMask & maskOfStatus;
}

