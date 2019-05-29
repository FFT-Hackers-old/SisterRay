#ifndef UPDATERS_H
#define UPDATERS_H

#include "containers.h"

void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void inventoryViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void inventoryViewQuantityUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void materiaNameViewUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex);
void materiaSphereViewUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex);
void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

void battleSpellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleSummonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleEskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleInventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
