#ifndef BATTLE_STAT_FORMULAS_H
#define BATTLE_STAT_FORMULAS_H

#include <SisterRay/SisterRay.h>

i32 srHPFormula(SrPartyData* character);
i32 srMPFormula(SrPartyData* character);
i32 srAtkPwrFormula(SrPartyData* character);
i32 srAtkSpdFormula(SrPartyData* character);
i32 srAccFormula(SrPartyData* actor);
i32 srCritRateFormula(SrPartyData* actor);
i32 srCritPwrFormula(SrPartyData* actor);
i32 srStgPwrFormula(SrPartyData* actor);
i32 srStgDmgFormula(SrPartyData* actor);

i32 srArmorFormula(SrPartyData* actor);
i32 srDefenseFormula(SrPartyData* actor);
i32 srEvadeFormula(SrPartyData* actor);
i32 srBlockAmtFormula(SrPartyData* actor);
i32 srBlockFormula(SrPartyData* actor);
i32 srParryFormula(SrPartyData* actor);
#endif

