#ifndef PARTY_UTILS_H
#define PARTY_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include <string>

bool characterCanEquipItem(u8 characterID, u16 materiaID);
u8* getMateriaSlots(u16 relativeEquipIdx, SrGameGearType gearType);
std::string getCharacterName(u8 characterID);
u8 getCharacterRecordIndex(u8 characterIdx);
PartyMember* getGamePartyMember(u8 actorIdx);
bool isPartyActor(u8 actorIdx);
bool actorHasStatus(u8 actorID, u32 maskOfStatus);

#endif
