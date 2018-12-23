#include <mog/mog.h>
#include <windows.h>
#include "damage_formulas.h"
#include "command_mechanics.h"
#include "impl.h"

PFNCOMMANDMAINPROC* oldCommandMain;
PFNRUNANIMSCRIPT* oldRunAnimationScript;
// PFNANIMEFFECT* oldAnimEffectLookUp;

__declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    oldCommandMain = mogRedirectFunction(MAIN_COMMAND_FORMULA, &CommandMainRewrite);
	oldRunAnimationScript = mogRedirectFunction(RUN_ANIMATION_SCRIPT, &AnimationScriptRewrite);
	//oldAnimEffectLookUp = mogRedirectFunction(LOOKUP_ADDITIONAL_EFFECT, &AnimationEffectRewrite);
    mogReplaceFunction(PHYSICAL_DAMAGE_FORMULA, &PhysicalFormulaRewrite);
    mogReplaceFunction(POISON_SETUP_ROUTINE, &ModifyPoisonTest);
}
