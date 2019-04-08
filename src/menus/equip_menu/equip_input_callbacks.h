#ifndef EQUIP_INPUT_CALLBACKS_H
#define EQUIP_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "equip_widget_names.h"

void equipGearHandler(const EquipInputEvent* event); //Signature of Listener Callback
void exitMenuListener(const EquipInputEvent* event);
void changeCharLeft(const EquipInputEvent* event);
void changeCharRight(const EquipInputEvent* event);
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex);
void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex);
u16 setupGearMenu(u8 itemType);

#endif
