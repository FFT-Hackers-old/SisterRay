#include <unordered_map>
#include <string>
#include "impl.h"
#include "api.h"

#define REGISTER(x) srRegisterFunction(#x, &x)

static std::unordered_map<std::string, const void*> gFuncReg;

SISTERRAY_API void initFunctionRegistry(void)
{
    REGISTER(srLoadFunction);
    REGISTER(srRegisterFunction);

    /*Register functions for accessing with game data*/
    REGISTER(getWeapon);
    REGISTER(setWeaponData);
    REGISTER(addWeapon);
    REGISTER(getArmor);
    REGISTER(setArmorData);
    REGISTER(addArmor);
    REGISTER(getAccessory);
    REGISTER(setAccessoryData);
    REGISTER(addAccessory);
    REGISTER(getItem);
    REGISTER(setItemData);
    REGISTER(addItem);
    REGISTER(getMateria);
    REGISTER(setMateriaData);
    REGISTER(addMateria);

    REGISTER(getMenu);
    REGISTER(addState);
    REGISTER(getStateCursor);
    REGISTER(setStateCursor);
    REGISTER(getMenuState);
    REGISTER(setMenuState);
    REGISTER(getWidget);

    REGISTER(srAddListener);
    REGISTER(srGetChild);
    REGISTER(moveWidget);
    REGISTER(enableWidget);
    REGISTER(disableWidget);
    REGISTER(getWidgetX);
    REGISTER(getWidgetY);

    REGISTER(setTextParams);
    REGISTER(srNewTextWidget);
    REGISTER(updateText);
    REGISTER(updateTextColor);
    REGISTER(updateTextPriority);
    REGISTER(getText);
    REGISTER(getTextColor);
    REGISTER(getTextPriority);

    REGISTER(setNumberParams);
    REGISTER(srNewNumberWidget);
    REGISTER(updateNumber);
    REGISTER(updateNumberColor);
    REGISTER(updateNumberPriority);
    REGISTER(getNumber);
    REGISTER(getNumberColor);
    REGISTER(getNumberPriority);

    REGISTER(srNewBoxWidget);
    REGISTER(setBoxParams);
    REGISTER(resizeBox);
    REGISTER(updateBoxPriority);
    REGISTER(getBoxDrawDistance);
    REGISTER(getBoxPriority);

    REGISTER(srNewPortraitWidget);
    REGISTER(setPortraitParams);
    REGISTER(updatePortraitPartyIndex);

    REGISTER(srNewHPBarWidget);
    REGISTER(setHPBarParams);
    REGISTER(updateHPBarPartyIndex);

    REGISTER(srNewSlotsWidget);
    REGISTER(setSlotsParams);
    REGISTER(updateMateriaSlots);

    REGISTER(srNewArrowWidget);
    REGISTER(setArrowParams);

    REGISTER(srNewGridWidget);

    /* Mog */
    srRegisterFunction("srRedirectFunction", &mogRedirectFunction);
    srRegisterFunction("srReplaceFunction", &mogReplaceFunction);
    srRegisterFunction("srReplaceNop", &mogReplaceNop);
    srRegisterFunction("srReplaceSkip", &mogReplaceSkip);
    srRegisterFunction("srVirtualAddress", &mogVirtualAddress);
}

SISTERRAY_API const void* srLoadFunction(const char* name)
{
    return gFuncReg[name];
}

SISTERRAY_API const void* srRegisterFunction(const char* name, const void* func)
{
    const void* old;

    old = srLoadFunction(name);
    gFuncReg[name] = func;
    return old;
}
