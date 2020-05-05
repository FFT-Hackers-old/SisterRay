#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE_H

#include <SisterRay/SisterRay.h>

/*Public API file, consumers can use these methods to add their own game data resources*/
SISTERRAY_API SrWeaponData getSrWeapon(u16 itemID, const char* modName);
SISTERRAY_API void setSrWeaponData(SrWeaponData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addSrWeapon(SrWeaponData attackData, u16 itemID, const char* modName, u8 characterID);

SISTERRAY_API SrArmorData getSrArmor(u16 itemID, const char* modName);
SISTERRAY_API void setSrArmorData(SrArmorData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addSrArmor(SrArmorData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrAccessoryData getSrAccessory(u16 itemID, const char* modName);
SISTERRAY_API void setSrAccessoryData(SrAccessoryData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addSrAccessory(SrAccessoryData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrConsumable getSrItem(u16 itemID, const char* modName);
SISTERRAY_API void setSrItemData(SrConsumable attackData, u16 itemID, const char* modName);
SISTERRAY_API void addSrItem(SrConsumable attackData, u16 itemID, const char* modName);

SISTERRAY_API SrMateriaData getSrMateria(u16 itemID, const char* modName);
SISTERRAY_API void setSrMateriaData(SrMateriaData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addSrMateria(SrMateriaData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrCommandData getSrCommand(u8 commandIdx, const char* modName);
SISTERRAY_API u8 getInternalCommandID(u8 commandIdx, const char* modName);
SISTERRAY_API void addSrCommand(SrCommandData commandData, u8 commandIdx, const char* modName);
SISTERRAY_API void addActionToCommand(const char* modName, u8 commandIdx, const char* actionModName, u16 actionIdx);
SISTERRAY_API void addSwapActionToCommand(const char* modName, u8 commandIdx, const char* actionModName, u16 actionIdx);
SISTERRAY_API void setActionEffectType(const char* modName, u16 modActionID, SrAnimationType effectType);
SISTERRAY_API u8 getCommandActionCount(const  char* modName, u8 modCmdIdx);
SISTERRAY_API void registerSelectCallback(const char* modName, u8 commandIdx, SRPFNCMDSELECTCALLBACK callback);
SISTERRAY_API void registerSetupCallback(const char* modName, u8 commandIdx, SRPFNCOMMANDSETUP callback);


SISTERRAY_API void addSrAction(SrActionData data, u16 modActionIdx, const char* modName);
SISTERRAY_API SrActionData getSrAction(u16 attackIdx, const char* modName);
SISTERRAY_API SrActionData getSrCommandAction(const char* modName, u8 modCmdIdx, u16 cmdAtkIdx);
SISTERRAY_API SrActionData getSrCommandSwapAction(const char* modName, u8 modCmdIdx, u16 cmdAtkIdx);
SISTERRAY_API void toggleSwapActions(const char* modName, u8 modCmdIdx, u16 enabledIdx);
SISTERRAY_API void addElementToAction(const char* modName, u16 modActionIdx, const char* elementName);
SISTERRAY_API void addStatusToAction(const char* modName, u16 modActionIdx, const char* statusName, u8 inflictionChance, u8 doesRemove, u8 doesToggle, u16 duration);
SISTERRAY_API void setActionAnimationScript(const char* modName, u16 modActionID, const char* modelName, const char* scriptModName, u16 modScriptID);
SISTERRAY_API void setEffectCallback(const char* modName, u16 modActionID, SRPFNSPELLEFFECTCALLBACK callback);

SISTERRAY_API bool maskIsSingleTarget(u16 targetMask);
SISTERRAY_API u8 getFirstMaskBitIdx(u16 targetMask);

#endif
