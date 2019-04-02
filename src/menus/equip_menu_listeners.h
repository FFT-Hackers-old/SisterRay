#ifndef EQUIP_MENU_LISTENERS
#define EQUIP_MENU_LISTENERS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "menu_utils.h"
#include "../widgets/widget.h"
#include "../widgets/primitives.h"
#include "../widgets/assets.h"

void equipGearHandler(const void* params);
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex);
void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex);

#endif
