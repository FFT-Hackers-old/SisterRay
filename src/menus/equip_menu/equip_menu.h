#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "equip_widget_names.h"
#include "equip_draw_callbacks.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask);
color getStatDisplayColor(u8 equippedStat, u8 toEquipStat);
void displayMenuCursors(CursorContext* cursorContextArray, u16 menuState, u32 stateControlMask);
void displayEquipMenuViews(i32 updateStateMask, Widget* equipWidget);
void handleEquipMenuInput(i32 updateStateMask, Menu* menuObject);
void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex);
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex);
u16 setupGearMenu(u8 itemType);

#endif
