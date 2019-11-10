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
    Menu* menuObject;
} EquipInitEvent;

typedef struct {
    Menu* menu;
    u32 menuState;
} InventoryDrawEvent;


typedef struct {
    Menu* menuObject;
} InventoryInitEvent;


typedef struct {
    Menu* menu;
    u32 menuState;
} MateriaDrawEvent;

typedef struct {
    Menu* menu;
    u32 menuState;
} MenuInputEvent;

typedef struct {
    Menu* menuObject;
} MateriaInitEvent;

typedef struct {
    Menu* menu;
    u32 menuState;
} MenuInputEvent;


typedef struct {
    Menu* menu;
    u32 menuState;
} BattleSpellDrawEvent;

typedef struct{};

typedef struct {
    Menu* menuObject;
} BattleInitEvent;

/*Event on which we dispatch handlers for displaying the various types of materia*/
typedef struct {
    Widget* displayWidget;
    MateriaInventoryEntry materia;
    u8 materiaLevel;
} DrawMateriaDataEvent;

#endif
