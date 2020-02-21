#ifndef DAMAGE_FORMULAS_H
#define DAMAGE_FORMULAS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"

typedef void(*PFNSR_DAMAGECALLBACK)(DamageCalculationEvent*);

class SrFormulaRegistry : public SrNamedResourceRegistry<PFNSR_DAMAGECALLBACK, std::string> {
public:
    SrFormulaRegistry() : SrNamedResourceRegistry<PFNSR_DAMAGECALLBACK, std::string>() {}
};

#endif // !DAMAGE_FORMULAS_H
