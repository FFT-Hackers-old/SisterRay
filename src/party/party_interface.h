#ifndef PARTY_INTERFACE_H
#define PARTY_INTERFACE_H

#include "party_actions.h"

SISTERRAY_API CharacterRecord* getPartyActorCharacterRecord(u8 partyIdx);
SISTERRAY_API SrCharacter* getSrCharacterRecord(u8 characterIdx);
SISTERRAY_API CharacterRecord* getGameCharacter(SrCharacter* srCharacter);

#endif
