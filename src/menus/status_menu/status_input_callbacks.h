#ifndef STATUS_INPUT_CALLBACKS_H
#define STATUS_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "status_widget_names.h"
#include "../../party/characters.h"

void exitMenuListener(const MenuInputEvent* event);
void changeCharLeft(const MenuInputEvent* event);
void changeCharRight(const MenuInputEvent* event);
#endif
