#ifndef MENU_EVENTS_H
#define MENU_EVENTS_H

#include "event.h"
#include "../widgets/widget.h"
#include "../menus/menu.h"

typedef struct {
    Widget* equipMenuWidget;
} EquipDrawEvent;


typedef struct {
    Widget* equipMenuWidget;
} EquipInputEvent;

typedef struct {
    Menu* menuObject;
} EquipInitEvent;

#endif
