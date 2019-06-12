#include <ctime>
#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "impl.h"
#include <zlib.h>
#include "inventories/inventory_functions.h"
#include "inventories/inventory_utils.h"
#include "menus/inventory_menu/inventory_menu.h"
#include "menus/inventory_menu/inventory_menu_callbacks.h"
#include "menus/equip_menu/equip_menu.h"
#include "menus/equip_menu//equip_menu_callbacks.h"
#include "menus/materia_menu/materia_menu.h"
#include "menus/materia_menu/materia_menu_callbacks.h"
#include "menus/battle_menu//battle_menu.h"
#include "party/party_callbacks.h"
#include "battle/ai_script_engine.h"
#include "battle/battle.h"
#include "battle/battle_context.h"


SrContext gContext;

static const SrKernelStreamHandler kKernelBinHandlers[9] = {
    initCommands,
    initAttacks,
    initCharacterData,
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
        { &gContext.gameStrings.command_descriptions,   0x20  },
        { &gContext.gameStrings.magic_descriptions,     0x100 },
        { &gContext.gameStrings.item_descriptions,      0x80  },
        { &gContext.gameStrings.weapon_descriptions,    0x80  },
        { &gContext.gameStrings.armor_descriptions,     0x20  },
        { &gContext.gameStrings.accessory_descriptions, 0x20  },
        { &gContext.gameStrings.materia_descriptions,   0x60  },
        { &gContext.gameStrings.key_item_descriptions,  0x40  },
        { &gContext.gameStrings.command_names,          0x20  },
        { &gContext.gameStrings.magic_names,            0x100 },
        { &gContext.gameStrings.item_names,             0x80  },
        { &gContext.gameStrings.weapon_names,           0x80  },
        { &gContext.gameStrings.armor_names,            0x20  },
        { &gContext.gameStrings.accessory_names,        0x20  },
        { &gContext.gameStrings.materia_names,          0x60  },
        { &gContext.gameStrings.key_item_names,         0x40  },
        { &gContext.gameStrings.battle_texts,           0x80  },
        { &gContext.gameStrings.summon_attack_names,    0x10  },
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
            srLogWrite("loading kernel2 string: %s", EncodedString(buffer + segStart + offsetTable[i]).str());
            entry.registry->add_resource(EncodedString(buffer + segStart + offsetTable[i]));
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


PFNRUNANIMSCRIPT* oldRunAnimationScript;

static void Init(void)
{
    initLog();
    srInitLua();
    initFunctionRegistry();
    initInventory();
    initBattleInventory();
    initMateriaInventory();
    initGameStrings();
    enableNoCD();
    srLoadKernel2Bin();
    srLoadKernelBin();
    initOnUseItemDataRegistry();
    initOnUseCallbackRegistry();
    initNoTargetCallbackRegistry();
    testFillInventory();
    initFormationsRegistries();; //initialize all data from the scene.bin
    //Register base callbacks
    initAnimations(); //Must be called after the formation registries have been initialized
    initAnimationScripts();
    registerEquipMenuListeners();
    initializeEquipMenu();
    registerInventoryMenuListeners();
    initializeInventoryMenu();
    registerMateriaMenuListeners();
    initializeMateriaMenu();
    registerPartyCallbacks();
    //End Register base callbacks, begin registering new handlers
    mogReplaceFunction(MAIN_INVENTORY_HANDLER, &inventoryMenuUpdateHandler); //add our new menu handler
    mogReplaceFunction(INIT_BATTLE_INVENTORY, &setupBattleInventory);
    /*mogReplaceFunction(RENDER_BATTLE_ITEM_MENU, &renderBattleItemView);
    mogReplaceFunction(INIT_BATTLE_ITEM_MENU_CURSOR, &initializeBattleItemMenuCursor);
    mogReplaceFunction(BATTLE_ITEM_MENU_INPUT_HANDLER, &battleItemMenuInputHandler);*/
    mogReplaceFunction(EQUIP_MENU_UPDATE_HANDLER, &equipMenuUpdateHandler);
    mogReplaceFunction(LOAD_ABILITY_DATA_HANDLER, &srLoadAbilityData);
    mogReplaceFunction(LOAD_FORMATION_HANDLER, &srLoadBattleFormation);
    mogReplaceFunction(EXECUTE_AI_SCRIPT_HANDLER, &srExecuteAIScript);
    mogReplaceFunction(EXECUTE_FORMATION_SCRIPT_HANDLER, &srExecuteFormationScripts);
    mogReplaceFunction(ENQUEUE_SCRIPT_ACTION, &enqueueScriptAction);
    mogReplaceFunction(TRANSFORM_ENEMY_COMMAND, &transformEnemyCommand);
    mogReplaceFunction(GET_MP_COST, &getMPCost);
    /*mogReplaceFunction(MAT_MATERIA_HANDLER, &materiaMenuUpdateHandler);
    mogReplaceFunction(RECALCULATE_DERIVED_STATS, &srRecalculateDerivedStats);
    mogReplaceFunction(DISPATCH_AUTO_ACTIONS, &dispatchAutoActions);
    mogReplaceFunction(UPDATE_COMMANDS_ACTIVE, &updateCommandsActive);
    mogReplaceFunction(DISPATCH_AUTO_ACTIONS, &dispatchAutoActions);
    initializeBattleMenu();*/
    oldRunAnimationScript = (PFNRUNANIMSCRIPT*)mogRedirectFunction(RUN_ANIMATION_SCRIPT, &animationScriptTrampoline);
    LoadMods();
    MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);

    /* Init RNG */
    srand((unsigned int)time(nullptr));

    /* Launch the lua console */
    srInitLuaConsole();
}

SISTERRAY_API __declspec(dllexport) void rayInit()
{
    /* Hook into WinMain (required for Win32 stuff) */
    mogReplaceNop((void*)0x67dbbb, 0x3e);
    mogInsertCall((void*)0x67dbbb, &Init);
}
