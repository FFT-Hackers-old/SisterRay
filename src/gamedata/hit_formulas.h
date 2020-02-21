#ifndef HIT_FORMULAS_H
#define HIT_FORMULAS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "damage_formulas.h"

class SrHitFormulaRegistry : public SrNamedResourceRegistry<PFNSR_DAMAGECALLBACK, std::string> {
public:
    SrHitFormulaRegistry() : SrNamedResourceRegistry<PFNSR_DAMAGECALLBACK, std::string>() {}
};

#endif // !DAMAGE_FORMULAS_H
