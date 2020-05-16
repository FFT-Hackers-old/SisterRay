#ifndef ATTACK_H
#define ATTACK_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "../EncodedString.h"
#include "../battle/actions/actions_api.h"
#include "statuses.h"
#include <unordered_map>
#include <string>
#include "battle_stats.h"
#include "formulas.h"
#include <unordered_set>
#include "gdata_utils.h"

struct SrAttack_ {
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
};

class SrAttackRegistry : public SrNamedResourceRegistry<SrAttack, std::string> {
public:
    SrAttackRegistry(SrKernelStream* stream);
    SrAttackRegistry() : SrNamedResourceRegistry<SrAttack, std::string>() {}
};

u16 getDefaultMagicUseMulti(u16 actionID);
void initializeActionElements(SrAttack& attack);
void initializeStatusAfflictions(SrAttack& attack);
SpellEffect getDefaultMagicMultiEffects(u16 actionID);
void setSummonSpellCtx(SrAttack& attack);

#endif
