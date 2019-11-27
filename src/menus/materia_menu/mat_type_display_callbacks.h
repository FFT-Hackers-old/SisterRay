#ifndef MAT_TYPE_DISPLAY_CALLBACKS_H
#define MAT_TYPE_DISPLAY_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "materia_widget_names.h"

void ablListH0S2(const DrawMateriaDataEvent* event);
void ablListH9S1(const DrawMateriaDataEvent* event);
void ablListH11S3(const DrawMateriaDataEvent* event);
void flagPercent(Widget* widget, u32 isEnabled);


#endif
