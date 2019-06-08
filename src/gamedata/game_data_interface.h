#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE

#include <SisterRay/SisterRay.h>

/*Public API file, consumers can use these methods to add their own game data resources*/
SISTERRAY_API WeaponData getWeapon(u16 itemID, const char* modName);
SISTERRAY_API void setWeaponData(WeaponData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addWeapon(WeaponData attackData, u16 itemID, const char* modName, u8 characterID);

SISTERRAY_API ArmorData getArmor(u16 itemID, const char* modName);
SISTERRAY_API void setArmorData(ArmorData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addArmor(ArmorData attackData, u16 itemID, const char* modName);

SISTERRAY_API AccessoryData getAccessory(u16 itemID, const char* modName);
SISTERRAY_API void setAccessoryData(AccessoryData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addAccessory(AccessoryData attackData, u16 itemID, const char* modName);

SISTERRAY_API ItemData getItem(u16 itemID, const char* modName);
SISTERRAY_API void setItemData(ItemData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addItem(ItemData attackData, u16 itemID, const char* modName);

SISTERRAY_API MateriaData getMateria(u16 itemID, const char* modName);
SISTERRAY_API void setMateriaData(MateriaData attackData, u16 itemID, const char* modName);
SISTERRAY_API void addMateria(MateriaData attackData, u16 itemID, const char* modName);

#endif
