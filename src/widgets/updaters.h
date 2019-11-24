#ifndef UPDATERS_H
#define UPDATERS_H

#include "containers.h"

void battleSpellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleSummonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleEskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void battleInventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
