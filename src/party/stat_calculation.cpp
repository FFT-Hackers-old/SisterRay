#include "stat_calculation.h"
#include "../impl.h"
#include "stat_names.h"

void calculateActorStats(SrPartyData& srPartyMember, const CharacterRecord& charRecord, const StatBoostModifiers& statModifiers) {
    for (auto& statElement : srPartyMember.playerStats) {
        const auto& statName = statElement.first;
        srLogWrite("Calculating stat %s", statName.c_str());
        auto& stat = statElement.second;
        auto& statData = gContext.stats.getElement(statName);
        stat.baseValue = getGameBaseStat(statName, charRecord);
        if (statModifiers.find(statName) == statModifiers.end()) {
            stat.statValue = stat.baseValue;
            srLogWrite("Defaulting stat %s to base value %d", statName.c_str(), stat.baseValue);
            continue;
        }
        const auto& modifiers = statModifiers.at(statName);
        stat.boosts = modifiers;
        i32 netBoostAmt = 0;
        i32 netPercentAmt = 0;
        for (const auto& modifier : modifiers) {
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
        //srLogWrite("Performing stat calculation: base: %d, percent: %d, boost:%d, max: %d", stat.baseValue, netPercentAmt, netBoostAmt, statData.maxValue);
        i32 statValue = (stat.baseValue * (1.0f + ((netPercentAmt) / 100.0f)) + netBoostAmt); //only apply percent to base value
        if (statValue > statData.maxValue) {
            stat.statValue = statData.maxValue;
            return;
        }
        if (!statData.canBeNegative && statValue < 0) {
            stat.statValue = 0;
            return;
        }
        //srLogWrite("Calculated stat value: %d for stat %s", statValue, statName.c_str());
        stat.statValue = statValue;
    }
}

u32 getGameBaseStat(std::string statName, const CharacterRecord& record) {
    if (statName == StatNames::HP)
        return record.base_HP;
    if (statName == StatNames::MP)
        return record.base_MP;
    if (statName == StatNames::STRENGTH)
        return record.strength + record.bonus_strength;
    if (statName == StatNames::VITALITY)
        return record.vitality + record.bonus_vitality;
    if (statName == StatNames::MAGIC)
        return record.magic + record.bonus_magic;
    if (statName == StatNames::SPIRIT)
        return record.spirit + record.bonus_spirit;
    if (statName == StatNames::DEXTERITY)
        return record.dexterity + record.bonus_dexterity;
    if (statName == StatNames::LUCK)
        return record.luck + record.luck;
    return 0;
}

void addStatBoosts(StatBoostModifiers& acc, const EquipmentStatBoosts& boosts) {
    for (auto element : boosts) {
        const auto& statName = element.first;
        for (auto boost : element.second) {
            acc[statName].push_back(boost);
        }
    }
}
