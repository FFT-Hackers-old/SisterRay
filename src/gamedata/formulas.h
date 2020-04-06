#ifndef DAMAGE_FORMULAS_H
#define DAMAGE_FORMULAS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include <unordered_set>

typedef void(*PFNSR_DAMAGECALLBACK)(DamageCalculationEvent*);

typedef enum {
    APPLY_BARRIERS,
    IGNORE_DEFENSE,
    CAN_CRIT
} DamageModifiers;

typedef enum {
    CHECK_ACCURACY,
    ALWAYS_HIT,
} HitModifiers;

struct DamageFormula_ {
    PFNSR_DAMAGECALLBACK baseFormulaCallback;
};


struct HitFormula_ {
    PFNSR_DAMAGECALLBACK baseFormulaCallback;
};

class SrFormulaRegistry : public SrNamedResourceRegistry<DamageFormula, std::string> {
public:
    SrFormulaRegistry() : SrNamedResourceRegistry<DamageFormula, std::string>() {}
};

class SrHitFormulaRegistry : public SrNamedResourceRegistry<HitFormula, std::string> {
public:
    SrHitFormulaRegistry() : SrNamedResourceRegistry<HitFormula, std::string>() {}
};


void initFormulas();
void srAttackDamageBase(DamageCalculationEvent* damageEvent);
void srSpellDamageBase(DamageCalculationEvent* damageEvent);
void srAttackHitBase(DamageCalculationEvent* damageEvent);
void srSpellHitBase(DamageCalculationEvent* damageEvent);

#endif // !DAMAGE_FORMULAS_H
