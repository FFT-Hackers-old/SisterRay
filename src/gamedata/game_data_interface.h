#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE

#include <SisterRay/SisterRay.h>

/*Public API file, consumers can use these methods to add their own game data resources*/
SISTERRAY_API SrWeaponData getWeapon(u16 itemID, const char* modName);
SISTERRAY_API void setWeaponData(SrWeaponData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addWeapon(SrWeaponData attackData, u16 itemID, const char* modName, u8 characterID);

SISTERRAY_API SrArmorData getArmor(u16 itemID, const char* modName);
SISTERRAY_API void setArmorData(SrArmorData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addArmor(SrArmorData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrAccessoryData getAccessory(u16 itemID, const char* modName);
SISTERRAY_API void setAccessoryData(SrAccessoryData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addAccessory(SrAccessoryData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrConsumableData getItem(u16 itemID, const char* modName);
SISTERRAY_API void setItemData(SrConsumableData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addItem(SrConsumableData attackData, u16 itemID, const char* modName);

SISTERRAY_API SrMateriaData getMateria(u16 itemID, const char* modName);
SISTERRAY_API void setMateriaData(SrMateriaData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addMateria(SrMateriaData attackData, u16 itemID, const char* modName);

SISTERRAY_API void addActionToCommand(const char* commandName, const char* actionName);

#endif
