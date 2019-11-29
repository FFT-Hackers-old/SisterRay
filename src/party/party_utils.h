#ifndef PARTY_UTILS_H
#define PARTY_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include <string>

u16 getEquippedGear(u8 characterID, u8 gearType);
bool characterCanEquipItem(u8 characterID, u16 item_id);
std::string getCharacterName(u8 characterID);
u16 getMateriaID(u8 characterID, u8 slot, u8 gearType);
u8 getCharacterRecordIndex(u8 partyIndex);
ActivePartyMemberStruct* getActivePartyMember(u8 actorIdx);
bool isPartyActor(u8 actorIdx);
bool actorHasStatus(u8 actorID, u32 maskOfStatus);

#endif
