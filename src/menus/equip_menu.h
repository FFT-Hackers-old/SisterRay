#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask);
void displayActiveCursorStates(i32 updateStateMask);
void displayEquipMenuViews(i32 updateStateMask);
void handleEquipMenuInput(i32 updateStateMask);
void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex);
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex);
u16 setupGearMenu(u8 itemType);

#endif
