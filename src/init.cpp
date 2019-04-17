#include <ctime>
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

struct Kernel2Entry {
    StringRegistry* registry;
    int             count;
};

static void srLoadKernel2Bin(void)
{
    static const Kernel2Entry kKernel2Entries[] = {
        { &gContext.game_strings.command_descriptions,   0x20  },
        { &gContext.game_strings.magic_descriptions,     0x100 },
        { &gContext.game_strings.item_descriptions,      0x80  },
        { &gContext.game_strings.weapon_descriptions,    0x80  },
        { &gContext.game_strings.armor_descriptions,     0x20  },
        { &gContext.game_strings.accessory_descriptions, 0x20  },
        { &gContext.game_strings.materia_descriptions,   0x60  },
        { &gContext.game_strings.key_item_descriptions,  0x40  },
        { &gContext.game_strings.command_names,          0x20  },
        { &gContext.game_strings.magic_names,            0x100 },
        { &gContext.game_strings.item_names,             0x80  },
        { &gContext.game_strings.weapon_names,           0x80  },
        { &gContext.game_strings.armor_names,            0x20  },
        { &gContext.game_strings.accessory_names,        0x20  },
        { &gContext.game_strings.materia_names,          0x60  },
        { &gContext.game_strings.key_item_names,         0x40  },
        { &gContext.game_strings.battle_texts,           0x80  },
        { &gContext.game_strings.summon_attack_names,    0x10  },
    };

    u16 offsetTable[0x100];

    FILE*   kernel2;
    char*   buffer;

    u32     segBase;
    u32     segLen;
    u32     segStart;

    buffer = new char[0x10000];

    kernel2 = fopen(srGetGamePath("data/kernel/kernel2.bin"), "rb");
    lzssDecompress(buffer, 0x10000, kernel2);
    fclose(kernel2);

    segBase = 0;

    for (const auto& entry : kKernel2Entries)
    {
        memcpy((char*)&segLen, buffer + segBase, 4);
        segStart = segBase + 4;
        memcpy((char*)&offsetTable, buffer + segStart, 2 * entry.count);
        for (int i = 0; i < entry.count; ++i)
        {
            entry.registry->add_resource(EncodedString(buffer + segStart + offsetTable[i]));
            srLogWrite("loading kernel2 string: %s", (buffer + segStart + offsetTable[i]));
        }
        segBase = segStart + segLen;
    }

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

static void hookWinMain(void)
{
    srand((unsigned int)time(nullptr));
    srInitLuaConsole();
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

    /* Hook into WinMain (required for Win32 stuff) */
    mogReplaceNop((void*)0x67dbbb, 0x3e);
    mogInsertCall((void*)0x67dbbb, &hookWinMain);
}
