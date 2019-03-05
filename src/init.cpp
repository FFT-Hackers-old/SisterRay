#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "impl.h"
#include <zlib.h>
#include "inventory_menu.h"
#include "inventory_functions.h"
#include "battle_menu.h"

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
    srLogWrite("Inventories initialized");
    init_game_strings();
    EnableNoCD();
    srLoadKernelBin();
    initOnUseDataRegistry();
    initOnUseCallbackRegistry();
    initNoTargetCallbackRegistry();
    testFillInventory();
    srLogWrite("test inventory filled");
    mogReplaceFunction(MAIN_INVENTORY_HANDLER, &inventoryMenuUpdateHandler); //add our new menu handler
    srLogWrite("Item handler replaced");
    mogReplaceFunction(INIT_INVENTORY_CURSOR_STATE, &onEnterInventory);
    srLogWrite("inventory menu initializer replaced");
    mogReplaceFunction(INIT_BATTLE_INVENTORY, &setupBattleInventory);
    mogReplaceFunction(RENDER_BATTLE_ITEM_MENU, &renderBattleItemView);
    mogReplaceFunction(INIT_BATTLE_ITEM_MENU_CURSOR, &initializeBattleItemMenuCursor);
    LoadMods();
    srLogWrite("mods successfully loaded");
}
