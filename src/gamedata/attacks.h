#ifndef ATTACK_H
#define ATTACK_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "../EncodedString.h"
#include "../battle/engine/action_spell_effects.h"
#include "statuses.h"
#include <unordered_map>
#include <string>
#include "../party/battle_stats.h"
#include "formulas.h"
#include <unordered_set>
#include "gdata_utils.h"

typedef struct {
    AttackData attackData;
    u16 attackID;
    EncodedString attackName;
    EncodedString attackDescription;
    SrAnimationType animationType;
    u16 animScriptIndex; //Overrides the command ID if set, -1 treated as null
    u8 useOverride;
    SpellEffect overrideEffect;
    u8 useMulti;
    SpellEffect multiEffect;
    std::vector<std::string> attackElements;
    std::vector<StatusInfliction> statusAttack;
    std::unordered_map<std::string, SrActorStat> attackStats; //For example status penetration
    bool useOverrideLimits;
    u16 hpDamageLimit;
    u16 mpDamageLimit;
    DamageType damageType;
    u16 damageFormula;
    std::unordered_set<DamageModifiers> dmgFormulaModifiers;
    u16 hitFormula;
    std::unordered_set<HitModifiers> hitFormulaModifiers;

} SrAttack;

class SrAttackRegistry : public SrNamedResourceRegistry<SrAttack, std::string> {
public:
    SrAttackRegistry(SrKernelStream* stream);
    SrAttackRegistry() : SrNamedResourceRegistry<SrAttack, std::string>() {}
};

void initAttacks(SrKernelStream* stream);
u16 getDefaultMagicUseMulti(u16 actionID);
void initializeActionElements(SrAttack& attack);
void initializeStatusAfflictions(SrAttack& attack);
SpellEffect getDefaultMagicMultiEffects(u16 actionID);

#endif
