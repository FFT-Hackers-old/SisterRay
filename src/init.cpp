#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "impl.h"
#include <zlib.h>
#include "inventories/inventory_functions.h"
#include "inventories/inventory_utils.h"
#include "menus/inventory_menu.h"
#include "menus/battle_menu.h"
#include "menus/equip_menu/equip_menu.h"
#include "menus/equip_menu//equip_menu_callbacks.h"

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

static void srLoadKernel2Bin(void)
{
    FILE* out;
    FILE* kernel2;
    char* buffer;
    uint32_t bufferLen;

    bufferLen = 65536;
    buffer = new char[bufferLen];

    kernel2 = fopen(srGetGamePath("data/kernel/kernel2.bin"), "rb");
    lzssDecompress(buffer, bufferLen, kernel2);
    fclose(kernel2);
    out = fopen("kernel2_decomp.bin", "wb");
    fwrite(buffer, bufferLen, 1, out);
    fclose(out);
    delete[] buffer;
}

static void srLoadKernelBin(void)
{
    FILE* kernel;
    SrKernelStream stream;
    SrKernelStreamHandler handler;
    initItemTypeData(); //initially allocate the mapping used for inventory_id -> relative resource id

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
    initLog();
    srInitLua();
    initFunctionRegistry();
    initInventory();
    initBattleInventory();
    initMateriaInventory();
    initGameStrings();
    enableNoCD();
    srLoadKernelBin();
    srLoadKernel2Bin();
    initOnUseDataRegistry();
    initOnUseCallbackRegistry();
    initNoTargetCallbackRegistry();
    testFillInventory();
    //Register base callbacks
    registerEquipMenuListeners();
    initializeEquipMenu();
    //End Register base callbacks
    mogReplaceFunction(MAIN_INVENTORY_HANDLER, &inventoryMenuUpdateHandler); //add our new menu handler
    mogReplaceFunction(INIT_INVENTORY_CURSOR_STATE, &onEnterInventory);
    mogReplaceFunction(INIT_BATTLE_INVENTORY, &setupBattleInventory);
    mogReplaceFunction(RENDER_BATTLE_ITEM_MENU, &renderBattleItemView);
    mogReplaceFunction(INIT_BATTLE_ITEM_MENU_CURSOR, &initializeBattleItemMenuCursor);
    mogReplaceFunction(BATTLE_ITEM_MENU_INPUT_HANDLER, &battleItemMenuInputHandler);
    mogReplaceFunction(EQUIP_MENU_UPDATE_HANDLER, &equipMenuUpdateHandler);
    LoadMods();
}
