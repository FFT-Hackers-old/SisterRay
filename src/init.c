#include <mog/mog.h>
#include <windows.h>
#include "../src/damage_formulas.h"

__declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    mogReplaceFunction(PHYSICAL_DAMAGE_FORMULA, &PhysicalFormulaRewrite);
}
