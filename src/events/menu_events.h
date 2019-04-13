#ifndef MENU_EVENTS_H
#define MENU_EVENTS_H

#include "event.h"
#include "../widgets/widget.h"
#include "../menus/menu.h"

typedef struct {
    Menu* menu;
    u32 menuState;
} EquipDrawEvent;


typedef struct {
    Widget* menu;
    u32 menuState;
} EquipInputEvent;

typedef struct {
    Menu* menuObject;
} EquipInitEvent;

#endif
