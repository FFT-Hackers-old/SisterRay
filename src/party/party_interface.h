#ifndef PARTY_INTERFACE_H
#define PARTY_INTERFACE_H

#include "party_actions.h"

SISTERRAY_API void enableActorCommand(u8 actorIdx, u8 enabledIndex, const char* modName, u8 modCmdIdx);
SISTERRAY_API void insertCommand(u8 characterIdx, u8 enabledIndex, u8 commandIndex, bool isTechnique=false);
SISTERRAY_API void setToggleCommand(u8 partyIdx, u8 enabledIndex, const char* modName, u8 modCmdIdx);
SISTERRAY_API void toggleBack(u8 partyIdx, u8 enabledIndex);
SISTERRAY_API CharacterRecord* getPartyActorCharacterRecord(u8 partyIdx);
SISTERRAY_API SrCharacter* getSrCharacterRecord(u8 characterIdx);
SISTERRAY_API CharacterRecord* getGameCharacter(SrCharacter* srCharacter);

#endif
