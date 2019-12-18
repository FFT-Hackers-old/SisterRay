#ifndef ATTACK_H
#define ATTACK_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "../EncodedString.h"
#include "../battle/engine/action_spell_effects.h"

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
} SrAttack;

class SrAttackRegistry : public SrNamedResourceRegistry<SrAttack, std::string> {
public:
    SrAttackRegistry(SrKernelStream* stream);
    SrAttackRegistry() : SrNamedResourceRegistry<SrAttack, std::string>() {}
};

void initAttacks(SrKernelStream* stream);
u16 getDefaultMagicUseMulti(u16 actionID);
SpellEffect getDefaultMagicMultiEffects(u16 actionID);

#endif
