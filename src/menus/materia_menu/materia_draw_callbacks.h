#ifndef MATERIA_DRAW_CALLBACKS
#define MATERIA_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"

void handleChangeCharacter(const MateriaDrawEvent* event);
void handleUpdateMateriaDescription(const MateriaDrawEvent* event);
void handleUpdateMateriaData(const MateriaDrawEvent* event);
void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia);
void updateEquipEffect(Widget* equipEffectWidget, MateriaInventoryEntry materia);
void updateAbilityListWidget(Widget* abilityListWidget, MateriaInventoryEntry materia, u8 level);
#endif
