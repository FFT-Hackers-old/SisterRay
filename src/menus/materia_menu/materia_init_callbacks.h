#ifndef MATERIA_INIT_CALLBACKS_H
#define MATERIA_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"
#include "../menu.h"

void initMateraCharDataWidget(const MateriaInitEvent* event);
void initMateriaDescWidget(const MateriaInitEvent* event);
void initMateriaViewWidget(const MateriaInitEvent* event);
void initMateriaDataWidget(const MateriaInitEvent* event);
void initCommandViewWidget(const MateriaInitEvent* event);
void initSpellViewWidget(const MateriaInitEvent* event);


Widget* allocateMateriaRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void materiaEntryUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex);

void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
