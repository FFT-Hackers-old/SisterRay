#ifndef DAMAGE_FORMULAS_H
#define DAMAGE_FORMULAS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include <vector>

typedef void(*PFNSR_DAMAGECALLBACK)(DamageCalculationEvent*);

typedef enum {
    APPLY_BARRIERS,
    IGNORE_DEFENSE,
    CAN_CRIT
} DamageModifiers;

typedef enum {
    CHECK_ACCURACY,
} HitModifiers;

struct DamageFormula_ {
    PFNSR_DAMAGECALLBACK baseFormulaCallback;
    std::vector<DamageModifiers> modifiers;
};


struct HitFormula_ {
    PFNSR_DAMAGECALLBACK baseFormulaCallback;
    std::vector<HitModifiers> modifiers;
};

class SrFormulaRegistry : public SrNamedResourceRegistry<DamageFormula, std::string> {
public:
    SrFormulaRegistry() : SrNamedResourceRegistry<DamageFormula, std::string>() {}
};

class SrHitFormulaRegistry : public SrNamedResourceRegistry<HitFormula, std::string> {
public:
    SrHitFormulaRegistry() : SrNamedResourceRegistry<HitFormula, std::string>() {}
};

#endif // !DAMAGE_FORMULAS_H
