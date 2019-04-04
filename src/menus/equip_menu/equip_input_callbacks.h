#ifndef EQUIP_INPUT_CALLBACKS
#define EQUIP_INPUT_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "equip_menu_widget.h"


typedef struct {
    Widget* equipMenuWidget;
} EquipInputEventParams;

void equipGearHandler(const void* params); //Signature of Listener Callback
void exitMenuListener(const void* params);
void changeCharLeft(const void* params);
void changeCharRight(const void* params);
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex);
void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex);
u16 setupGearMenu(u8 itemType);

#endif
