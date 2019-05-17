#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE

#include <SisterRay/SisterRay.h>

SISTERRAY_API WeaponData getWeapon(u16 itemID);
SISTERRAY_API void setWeaponData(WeaponData attackData, u16 itemID);
SISTERRAY_API void addWeapon(WeaponData attackData, char* name);

SISTERRAY_API ArmorData getArmor(u16 itemID);
SISTERRAY_API void setArmorData(ArmorData attackData, u16 itemID);
SISTERRAY_API void addArmor(ArmorData attackData, char* name);

SISTERRAY_API AccessoryData getAccessory(u16 itemID);
SISTERRAY_API void setAccessoryData(AccessoryData attackData, u16 itemID);
SISTERRAY_API void addAccessory(AccessoryData attackData, char* name);

SISTERRAY_API ItemData getItem(u16 itemID);
SISTERRAY_API void setItemData(ItemData attackData, u16 itemID);
SISTERRAY_API void addItem(ItemData attackData, char* name);

SISTERRAY_API MateriaData getMateria(u16 itemID);
SISTERRAY_API void setMateriaData(MateriaData attackData, u16 itemID);
SISTERRAY_API void addMateria(MateriaData attackData, char* name);

#endif
