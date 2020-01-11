#include "stat_boosts.h"
#include "../impl.h"

void calculateActorStats(SrPartyData& srPartyMember, const CharacterRecord& charRecord, const StatBoostModifiers& statModifiers) {
    for (auto statElement : srPartyMember.playerStats) {
        const auto& statName = statElement.first;
        auto& stat = statElement.second;
        stat.baseValue = getGameBaseStat(statName, charRecord);
        const auto& modifiers = statModifiers[statName];
        i32 netBoostAmt = 0;
        i32 netPercentAmt = 0;
        for (auto modifier : modifiers) {
            if (modifier.sign) {
                if (modifier.isPercent)
                    netPercentAmt -= modifier.amount;
                else
                    netBoostAmt -= modifier.amount;
            }
            else {
                if (modifier.isPercent)
                    netPercentAmt += modifier.amount;
                else
                    netBoostAmt += modifier.amount;
            }
        }

        i32 statValue = (stat.baseValue * (1.0f + ((netPercentAmt) / 100)) + netBoostAmt; //only apply percent to base value
        if (statValue > stat.maxValue) {
            stat.statValue = stat.maxValue;
            return;
        }
        if (statValue < 0) {
            stat.statValue = 0;
            return;
        }
        stat.statValue = statValue;
    }
}

u8 getGameBaseStat(std::string statName, const CharacterRecord& record) {
    if (statName == "HP")
        return record.base_HP;
    if (statName == "MP")
        return record.base_MP;
    if (statName == "STR")
        return record.strength + record.bonus_strength;
    if (statName == "VIT")
        return record.vitality + record.bonus_vitality;
    if (statName == "MAG")
        return record.magic + record.bonus_magic;
    if (statName == "SPR")
        return record.spirit + record.bonus_spirit;
    if (statName == "DEX")
        return record.dexterity + record.bonus_dexterity;
    if (statName == "LCK")
        return record.luck + record.bonus_luck;
}

void addStatBoosts(StatBoostModifiers& acc, const EquipmentStatBoosts& boosts) {
    for (auto element : boosts) {
        const auto& statName = element.first;
        for (auto boost : element.second) {
            acc[statName].push_back(boost);
        }
    }
}
