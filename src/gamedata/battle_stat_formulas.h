#ifndef BATTLE_STAT_FORMULAS_H
#define BATTLE_STAT_FORMULAS_H

#include <SisterRay/SisterRay.h>

i32 srHPFormula(StatFormulaCtx* character);
i32 srMPFormula(StatFormulaCtx* character);
i32 srAtkPwrFormula(StatFormulaCtx* character);
i32 srAtkSpdFormula(StatFormulaCtx* character);
i32 srAccFormula(StatFormulaCtx* actor);
i32 srCritRateFormula(StatFormulaCtx* actor);
i32 srCritPwrFormula(StatFormulaCtx* actor);
i32 srStgPwrFormula(StatFormulaCtx* actor);
i32 srStgDmgFormula(StatFormulaCtx* actor);

i32 srArmorFormula(StatFormulaCtx* actor);
i32 srDefenseFormula(StatFormulaCtx* actor);
i32 srEvadeFormula(StatFormulaCtx* actor);
i32 srBlockAmtFormula(StatFormulaCtx* actor);
i32 srBlockFormula(StatFormulaCtx* actor);
i32 srParryFormula(StatFormulaCtx* actor);
#endif

