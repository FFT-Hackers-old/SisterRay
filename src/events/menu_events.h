#ifndef MENU_EVENTS_H
#define MENU_EVENTS_H

#include "event.h"
#include "../widgets/widget.h"
#include "../menus/menu.h"

typedef struct {
    Menu* menu;
    u32 menuState;
    i32 updateStateMask;
} MenuDrawEvent;

typedef struct {
    Menu* menu;
} MenuInitEvent;


typedef struct {
    Menu* menu;
    u32 menuState;
} MenuInputEvent;

/*Event on which we dispatch handlers for displaying the various types of materia*/
typedef struct {
    Widget* displayWidget;
    MateriaInventoryEntry materia;
    u8 materiaLevel;
} DrawMateriaDataEvent;

#endif
