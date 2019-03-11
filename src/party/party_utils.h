#ifndef PARTY_UTILS_H
#define PARTY_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

u16 getEquippedGear(u8 characterID, u8 gearType);
bool characterCanEquipItem(u8 characterID, u16 item_id);

#endif