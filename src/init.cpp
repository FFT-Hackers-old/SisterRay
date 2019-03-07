#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "impl.h"
#include <zlib.h>
#include "menus/inventory_menu.h"
#include "inventories/inventory_functions.h"
#include "inventories/inventory_utils.h"
#include "menus/battle_menu.h"

SrContext gContext;

static const SrKernelStreamHandler kKernelBinHandlers[9] = {
    NULL,
    NULL,
    NULL,
    NULL,
    initItems,
    init_weapon,
    init_armor,
    init_accessory,
    init_materia,
};

static void srLoadKernelBin(void)
{
    FILE* kernel;
    SrKernelStream stream;
    SrKernelStreamHandler handler;
    init_item_type_data(); //initially allocate the mapping used for inventory_id -> relative resource id

    kernel = fopen(srGetGamePath("data/kernel/kernel.bin"), "rb");
    for (int i = 0; i < 9; ++i)
    {
        srKernelStreamOpen(&stream, kernel);
        handler = kKernelBinHandlers[i];
        if (handler)
            handler(&stream);
        else
            srKernelStreamSkip(&stream);
        srKernelStreamClose(&stream);
    }
    fclose(kernel);
}

SISTERRAY_API __declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    InitLog();
    InitFunctionRegistry();
    InitInventory();
    InitBattleInventory();
    init_game_strings();
    EnableNoCD();
    srLoadKernelBin();
    initOnUseDataRegistry();
    initOnUseCallbackRegistry();
    initNoTargetCallbackRegistry();
    testFillInventory();
    mogReplaceFunction(MAIN_INVENTORY_HANDLER, &inventoryMenuUpdateHandler); //add our new menu handler
    mogReplaceFunction(INIT_INVENTORY_CURSOR_STATE, &onEnterInventory);
    mogReplaceFunction(INIT_BATTLE_INVENTORY, &setupBattleInventory);
    mogReplaceFunction(RENDER_BATTLE_ITEM_MENU, &renderBattleItemView);
    mogReplaceFunction(INIT_BATTLE_ITEM_MENU_CURSOR, &initializeBattleItemMenuCursor);
    mogReplaceFunction(BATTLE_ITEM_MENU_INPUT_HANDLER, &battleItemMenuInputHandler);
    LoadMods();
}
