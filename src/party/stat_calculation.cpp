#include "stat_calculation.h"
#include "../impl.h"
#include "../gamedata/stat_names.h"
#include "../gamedata/battle_stats.h"


void calculatePrimaryBaseStats(const SrCharacter& character, SrPartyData& srPartyMember) {
    for (auto& statElement : gContext.stats.named_registry) {
        auto& statName = statElement.first;
        auto& stat = gContext.stats.getResource(statElement.second);
        if (stat.isPrimary) {
            if (stat.useTable) {
                auto val = character.statTable.at(statName).at(character.gameCharacter->level) + (character.statPointMultipliers.at(statName) * character.appliedPoints.at(statName));
                srLogWrite("Setting primary table stat value to %i", val);
                setStat(statName.c_str(), &srPartyMember.stats[statName].baseValue, val);
            }
        }
    }
}

void calculateDerivedBaseStats(const SrCharacter& character, SrPartyData& srPartyMember){
    for (auto& statElement : gContext.stats.named_registry) {
        auto& statName = statElement.first;
        auto& stat = gContext.stats.getResource(statElement.second);
        if (stat.isDerived) {
            if (stat.useTable) {
                auto val = character.statTable.at(statName).at(character.gameCharacter->level) + (character.statPointMultipliers.at(statName) * character.appliedPoints.at(statName));
                srLogWrite("Setting derived table stat value to %i", val);
                setStat(statName.c_str(), &srPartyMember.stats[statName].baseValue, val);
            }
            if (stat.derivedFormula) {
                StatFormulaCtx ctx{ &(srPartyMember.stats), true, 0, false, false };
                srLogWrite("Setting derived forumla stat value to %i", stat.derivedFormula(&ctx));
                setStat(statName.c_str(), &srPartyMember.stats[statName].baseValue, stat.derivedFormula(&ctx));
            }
        }
    }
}

void applyStatBoosts(SrPartyData& srPartyMember, const StatBoostModifiers& statModifiers, bool calculatePrimary) {
    for (auto& statElement : srPartyMember.stats) {
        const auto& statName = statElement.first;
        srLogWrite("Calculating stat %s", statName.c_str());
        auto& stat = statElement.second;

        const auto& statData = gContext.stats.getElement(statName);
        if (calculatePrimary && !statData.isPrimary) {
            continue;
        }
        if (!calculatePrimary && statData.isPrimary) {
            continue;
        }
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

void addStatBoosts(StatBoostModifiers& acc, const EquipmentStatBoosts& boosts) {
    for (auto element : boosts) {
        const auto& statName = element.first;
        for (auto boost : element.second) {
            acc[statName].push_back(boost);
        }
    }
}
