#include <mog/mog.h>
#include <windows.h>
#include "../src/damage_formulas.h"
#include <../src/command_mechanics.h>

__declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    mogReplaceFunction(PHYSICAL_DAMAGE_FORMULA, &PhysicalFormulaRewrite);
    mogReplaceFunction(MAIN_COMMAND_FORMULA, &CommandMainRewrite);
}
