#ifndef BATTLE_TECHNIQUE_INIT_CALLBACKS_H
#define BATTLE_TECHNIQUE_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"
#include "../battle_menu_widget_names.h"


void initBattleTechniqueViewWidget(const MenuInitEvent* event);
void battleTechniqueNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);



#endif
