#include "menu_engine.h"
#include "equip_menu/equip_menu_callbacks.h"
#include "materia_menu/materia_menu_callbacks.h"
#include "inventory_menu/inventory_menu_callbacks.h"
#include "status_menu/status_menu_callbacks.h"
#include "../impl.h"

#define STATUS_MATERIA_HANDLER   ((void*)0x703ABD)

void initializeSrMenuEngine() {
    mogReplaceFunction(MAIN_INVENTORY_HANDLER, &inventoryMenuUpdateHandler); //add our new menu handler
//mogReplaceFunction(INIT_BATTLE_INVENTORY, &setupBattleInventory);
    mogReplaceFunction(EQUIP_MENU_UPDATE_HANDLER, &equipMenuUpdateHandler);
    mogReplaceFunction(MAT_MATERIA_HANDLER, &materiaMenuUpdateHandler);
    mogReplaceFunction(STATUS_MATERIA_HANDLER, &statusMenuUpdateHandler);

    registerEquipMenuListeners();
    initializeEquipMenu();
    registerInventoryMenuListeners();
    initializeInventoryMenu();
    registerMateriaMenuListeners();
    initializeMateriaMenu();
}
