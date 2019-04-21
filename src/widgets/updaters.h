#ifndef UPDATERS_H
#define UPDATERS_H

#include "containers.h"

void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void inventoryViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void inventoryViewQuantityUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
