#ifndef STATUS_INIT_CALLBACKS_H
#define STATUS_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../menu.h"

void initStatCharDataWidget(const MenuInitEvent* event);
void initPrimaryStatsView(const MenuInitEvent* event);
void initStatCombatDataWidget(const MenuInitEvent* event);
Widget* allocateStatRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void primaryStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void statusCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void statEquipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void equipmentStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void combatStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
#endif
