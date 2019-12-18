#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE

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
SISTERRAY_API void addSrCommand(SrCommandData commandData, u8 commandIdx, const char* modName);
SISTERRAY_API void registerSelectCallback(const char* name, SRPFNCMDSELECTCALLBACK callback);
SISTERRAY_API void registerSetupCallback(const char* name, SRPFNCOMMANDSETUP callback);
SISTERRAY_API void addActionToCommand(const char* commandName, const char* actionName);

#endif
