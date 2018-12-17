#include <mog/mog.h>
#include <windows.h>
#include "damage_formulas.h"
#include "command_mechanics.h"
#include "impl.h"

PFNCOMMANDMAINPROC* oldCommandMain;

__declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    oldCommandMain = mogRedirectFunction(MAIN_COMMAND_FORMULA, &CommandMainRewrite);
    mogReplaceFunction(PHYSICAL_DAMAGE_FORMULA, &PhysicalFormulaRewrite);
}
