#ifndef WEAPONS_H
#define WEAPONS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"
#include <vector>
#include "statuses.h"
#include "formulas.h"

#define KERNEL_WEAPON_COUNT 128

struct SrWeapon_ {
    WeaponData gameWeapon;
    AuxWeaponData auxData;
    EncodedString weaponName;
    EncodedString weaponDescription;
    std::unordered_map<std::string, SrActorStat> weaponStats;
    EquipmentStatBoosts equipEffects;
    std::vector<std::string> attackElements;
    std::vector<StatusInfliction> statusAttack;
    DamageType damageType;
    u16 damageFormula;
    std::unordered_set<DamageModifiers> dmgFormulaModifiers;
    u16 hitFormula;
    std::unordered_set<HitModifiers> hitFormulaModifiers;
    u16 weaponModelID;
};
 
class SrWeaponRegistry : public SrNamedResourceRegistry<SrWeapon, std::string> {
public:
    SrWeaponRegistry(SrKernelStream* stream);
    SrWeaponRegistry() : SrNamedResourceRegistry<SrWeapon, std::string>() {}
};

u8 getWeaponIcon(u8 characterID);
void initializeWeaponElements(SrWeapon& weapon);
void initializeWeaponAfflictions(SrWeapon& weapon);

#endif
