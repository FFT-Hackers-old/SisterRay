#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>
#include "../EncodedString.h"
#include "party_member.h"
#include "battle_stats.h"
#include "../gamedata/base_item.h"
#include <string>

void calculateActorStats(SrPartyData& srPartyMember, const CharacterRecord& charRecord, const StatBoostModifiers& statModifiers);
u8 getGameBaseStat(std::string statName, const CharacterRecord& record);
void addStatBoosts(StatBoostModifiers& acc, const EquipmentStatBoosts& boosts);


#endif
