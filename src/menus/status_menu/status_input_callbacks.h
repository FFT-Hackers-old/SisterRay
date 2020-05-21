#ifndef STATUS_INPUT_CALLBACKS_H
#define STATUS_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "status_widget_names.h"
#include "../../party/characters.h"

void statExitMenuListener(const MenuInputEvent* event);
void statChangeCharLeft(const MenuInputEvent* event);
void statChangeCharRight(const MenuInputEvent* event);
#endif
