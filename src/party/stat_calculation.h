#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>
#include "../EncodedString.h"
#include "party_member.h"
#include "../gamedata/battle_stats.h"
#include "../gamedata/base_item.h"
#include <string>

using StatMap = std::unordered_map<std::string, SrBoostedStat>;

void calculatePrimaryBaseStats(const SrCharacter& character, SrPartyData& srPartyMember);
void calculateDerivedBaseStats(const SrCharacter& character, SrPartyData& srPartyMember);
void applyStatBoosts(SrPartyData& srPartyMember, const StatBoostModifiers& statModifiers, bool calculatePrimary);
void addStatBoosts(StatBoostModifiers& acc, const EquipmentStatBoosts& boosts);


#endif
