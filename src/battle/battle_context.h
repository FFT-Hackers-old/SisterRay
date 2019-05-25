#ifndef BATTLE_CONTEXT_H
#define BATTLE_CONTEXT_H

#include <SisterRay/SisterRay.h>
#include <string>

void srLoadAbilityData();
void updatePlayerSpellData(EnabledSpell* spellData, const AttackData& abilityData);
AttackData initializeLimitContext(const std::string& abilityKey);
void setStatusInflictionData(i32 statusInflictionByte, i32 inflictedStatusMask);

#endif
