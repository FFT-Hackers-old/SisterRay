#ifndef MATERIA_DRAW_CALLBACKS
#define MATERIA_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"
#include "../../gamedata/materia.h"

void handleChangeMateriaCharacter(const MenuDrawEvent* event);
void handleUpdateMateriaDescription(const MenuDrawEvent* event);
void handleUpdateMateriaData(const MenuDrawEvent* event);
void handleUpdateMateriaView(const MenuDrawEvent* event);
void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia);
void dispatchMateriaTypeHandlers(Widget* displayWidget, const MateriaInventoryEntry& materia, u8 materiaLevel);
void drawCommandViewWidget(const MenuDrawEvent* event);
void drawSpellsWidget(const MenuDrawEvent* event);
#endif
