#ifndef GDATA_UTILS_H
#define GDATA_UTILS_H

#include <SisterRay/SisterRay.h>
#include <string>
#include "formula_names.h"


const std::string assembleGDataKey(u16 resourceIdx);

//For use with Weapons, Items, Attacks, anything SrObject with a damagebyte
template <class T> void setSrDamageInfo(T& dmgResource, u8 damageByte) {
    u8 highDmg = (damageByte >> 4) & 0xF0;
    u8 lowDmg = damageByte & 0xF;
    switch (highDmg) {
    case 0: {
        dmgResource.damageType = PHYSICAL;
        setSrDamageLow<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(CAN_CRIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.hitFormulaModifiers.insert(ALWAYS_HIT);
        break;
    }
    case 1: {
        dmgResource.damageType = PHYSICAL;
        setSrDamageLow<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(CAN_CRIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.hitFormulaModifiers.insert(CHECK_ACCURACY);
        break;
    }
    case 2: {
        dmgResource.damageType = MAGICAL;
        setSrDamageLow<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::MAGICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.hitFormulaModifiers.insert(CHECK_ACCURACY);
        break;
    }
    case 3: {
        dmgResource.damageType = PHYSICAL;
        setSrDamageLow<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_HIT);
        dmgResource.hitFormulaModifiers.insert(ALWAYS_HIT);
        break;
    }
    case 4:
    case 5: {
        dmgResource.damageType = MAGICAL;
        setSrDamageLow<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::MAGICAL_BASE_HIT);
        dmgResource.hitFormulaModifiers.insert(ALWAYS_HIT);
        break;
    }
    case 6: {
        dmgResource.damageType = PHYSICAL;
        setSrDamageSpecial<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(CAN_CRIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.dmgFormulaModifiers.insert(IGNORE_DEFENSE);
        dmgResource.hitFormulaModifiers.insert(CHECK_ACCURACY);
        break;
    }
    case 7: {
        dmgResource.damageType = MAGICAL;
        setSrDamageSpecial<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::MAGICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.dmgFormulaModifiers.insert(IGNORE_DEFENSE);
        dmgResource.hitFormulaModifiers.insert(CHECK_ACCURACY);
        break;
    }
    case 8: {
        break;
    }
    case 9: {
        break;
    }
    case 0xA: {
        dmgResource.damageType = PHYSICAL;
        setSrDamageUltimateMods<T>(lowDmg, dmgResource);
        dmgResource.hitFormula = gContext.hitFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_HIT);
        dmgResource.dmgFormulaModifiers.insert(CAN_CRIT);
        dmgResource.dmgFormulaModifiers.insert(APPLY_BARRIERS);
        dmgResource.hitFormulaModifiers.insert(CHECK_ACCURACY);
        break;
    }
    }
}

template<class T> void setSrDamageLow(u8 lowType, T& dmgResource) {
    switch (lowType) {
    case 0: {
        dmgResource.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    case 1: {
        dmgResource.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    }
}

template<class T> void setSrDamageSpecial(u8 lowType, T& attack) {
    switch (lowType) {
    case 0: {
        attack.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    case 1: {
        attack.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    }
}

template<class T> void setSrDamageUltimateMods(u8 lowType, T& attack) {
    switch (lowType) {
    case 0: {
        attack.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    case 1: {
        attack.damageFormula = gContext.damageFormulas.getResourceIndex(FormulaNames::PHYSICAL_BASE_DMG);
    }
    }
}
#endif // !GDATA_UTILS_H
