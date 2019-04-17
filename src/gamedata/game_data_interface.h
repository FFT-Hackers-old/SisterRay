#ifndef GAME_DATA_INTERFACE_H
#define GAME_DATA_INTERFACE

#include <SisterRay/SisterRay.h>

SISTERRAY_API WeaponData getWeapon(u16 itemID);
SISTERRAY_API void setWeaponData(WeaponData data, u16 itemID);
SISTERRAY_API void addWeapon(WeaponData data, char* name);

SISTERRAY_API ArmorData getArmor(u16 itemID);
SISTERRAY_API void setArmorData(ArmorData data, u16 itemID);
SISTERRAY_API void addArmor(ArmorData data, char* name);

SISTERRAY_API AccessoryData getAccessory(u16 itemID);
SISTERRAY_API void setAccessoryData(AccessoryData data, u16 itemID);
SISTERRAY_API void addAccessory(AccessoryData data, char* name);

SISTERRAY_API ItemData getItem(u16 itemID);
SISTERRAY_API void setItemData(ItemData data, u16 itemID);
SISTERRAY_API void addItem(ItemData data, char* name);

SISTERRAY_API MateriaData getMateria(u16 itemID);
SISTERRAY_API void setMateriaData(MateriaData data, u16 itemID);
SISTERRAY_API void addMateria(MateriaData data, char* name);

#endif
