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

#endif
