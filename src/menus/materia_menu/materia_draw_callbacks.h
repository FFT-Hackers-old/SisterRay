#ifndef MATERIA_DRAW_CALLBACKS
#define MATERIA_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"
#include "../../gamedata/materia.h"

void handleChangeCharacter(const MateriaDrawEvent* event);
void handleUpdateMateriaDescription(const MateriaDrawEvent* event);
void handleUpdateMateriaData(const MateriaDrawEvent* event);
void handleUpdateMateriaView(const MateriaDrawEvent* event);
void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia);
void dispatchMateriaTypeHandlers(Widget* displayWidget, const MateriaInventoryEntry& materia, u8 materiaLevel);
void drawCommandViewWidget(const MateriaDrawEvent* event);
void drawSpellsWidget(const MateriaDrawEvent* event);
#endif
