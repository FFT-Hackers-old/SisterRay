#ifndef MATERIA_INIT_CALLBACKS_H
#define MATERIA_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "materia_widget_names.h"
#include "../menu.h"

void initMateraCharDataWidget(const MenuInitEvent* event);
void initMateriaDescWidget(const MenuInitEvent* event);
void initMateriaViewWidget(const MenuInitEvent* event);
void initMateriaDataWidget(const MenuInitEvent* event);
void initCommandViewWidget(const MenuInitEvent* event);
void initSpellViewWidget(const MenuInitEvent* event);


Widget* allocateMateriaRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void materiaEntryUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex);


Widget* allocateOptionsRow(const char* name, i32 x, i32 y);
Widget* allocateMateriaEquipRow(const char* name, i32 x, i32 y);
void matEquipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void matCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void matTechniqueViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
