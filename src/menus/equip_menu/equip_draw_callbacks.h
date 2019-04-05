#ifndef EQUIP_DRAW_CALLBACKS
#define EQUIP_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "equip_menu_widget.h"


typedef struct {
    Widget* equipMenuWidget;
} EquipDrawEventParams;


#endif
void handleChangeCharacter(const EquipDrawEventParams* params);
void handleUpdateDescription(const EquipDrawEventParams* params);
void handleUpdateGearSlotsWidget(const EquipDrawEventParams* params);
void handleUpdateStatMenuWidget(const EquipDrawEventParams* params);
