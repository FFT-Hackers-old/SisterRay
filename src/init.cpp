#include <ctime>
#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "impl.h"
#include <zlib.h>
#include "inventories/inventory_functions.h"
#include "inventories/inventory_utils.h"
#include "menus/menu_engine.h"
#include "menus/battle_menu//battle_menu.h"
#include "party/party_callbacks.h"
#include "battle/battle_engine_api.h"
#include "battle/battle.h"
#include "files/lgp_loader.h"
#include "battle/sr_battle_engine.h"
#include "gamedata/command_sr_callbacks.h"
#include "mods/controllable_summon.h"
#include "gamedata/limits.h"
#include "gamedata/summons.h"
#include <random>


SrContext gContext;
PRNG_Type rng;
std::uniform_int_distribution<PRNG_Type::result_type> udist(0, 255);

void finalizeRegistries() {
    finalizeArmors();
    finalizeWeapons();
    finalizeEnemies();
    finalizeAttacks();
    finalizeCommands();
}

static const SrKernelStreamHandler kKernelBinHandlers[9] = {
    initCommands,
    initAttacks,
    initCharacterData,
    NULL,
    initItems,
    initWeapons,
    initArmor,
    initAccessories,
    initMateria,
};

struct Kernel2Entry {
    StringRegistry* registry;
    int             count;
};

static void srLoadKernel2Bin(void) {
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

    for (const auto& entry : kKernel2Entries) {
        memcpy((char*)&segLen, buffer + segBase, 4);
        segStart = segBase + 4;
        memcpy((char*)&offsetTable, buffer + segStart, 2 * entry.count);
        for (int i = 0; i < entry.count; ++i) {
            srLogWrite("loading kernel2 string: %s", EncodedString(buffer + segStart + offsetTable[i]).str());
            entry.registry->addResource(EncodedString(buffer + segStart + offsetTable[i]));
        }
        segBase = segStart + segLen;
    }

    delete[] buffer;
}

static void srLoadKernelBin(void) {
    FILE* kernel;
    SrKernelStream stream;
    SrKernelStreamHandler handler;
    initBaseItems(); //initially allocate the mapping used for inventory_id -> relative resource id

    kernel = fopen(srGetGamePath("data/kernel/kernel.bin"), "rb");
    for (int i = 0; i < 9; ++i) {
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

static void Init(void) {
    MessageBoxA(NULL, "Sister Ray drawing power...", "SisterRay", 0);
    std::random_device seeder;
    PRNG_Type::result_type const seedval = seeder(); 
    rng.seed(seedval);
    initLog();
    //srInitLua();
    initItemTypes();
    initFunctionRegistry();
    initInventory();
    initBattleInventory();
    initMateriaInventory();
    initGameStrings();
    initStats(true);
    initStatuses();
    initElements();
    initFormulas();
    srLoadKernel2Bin();
    srLoadKernelBin();
    initParty();
    initOnUseCallbackRegistry();
    initNoTargetCallbackRegistry();
    testFillInventory();
    initFormationsRegistries();
    auto battleLGP = readLGPArchive(srGetGamePath(BATTLE_LGP_PATH));
    auto magicLGP = readLGPArchive(srGetGamePath(MAGIC_LGP_PATH));
    initAnimations(battleLGP, magicLGP); //Must be called after the formation registries have been initialized
    initAnimationScripts(battleLGP);
    initAnimScriptOpCodes();
    initSummons((u8*)magicLGP);
    loadControllableSummon();
    initLimits((u8*)magicLGP);
    free(battleLGP);
    free(magicLGP);

    srLogWrite("menus initialization complete");
    //End Register base callbacks, begin registering new handlers
    mogReplaceFunction(LOAD_ABILITY_DATA_HANDLER, &srLoadAbilityData);
    mogReplaceFunction(EXECUTE_AI_SCRIPT_HANDLER, &srExecuteAIScript);
    mogReplaceFunction(EXECUTE_FORMATION_SCRIPT_HANDLER, &srExecuteFormationScripts);
    mogReplaceFunction(ENQUEUE_SCRIPT_ACTION, &enqueueScriptAction);
    mogReplaceFunction(GET_MP_COST, &getMPCost);
    mogReplaceFunction(RECALCULATE_DERIVED_STATS, &srRecalculateDerivedStats);
    mogReplaceFunction(DISPATCH_AUTO_ACTIONS, &dispatchAutoActions);
    mogReplaceFunction(UPDATE_COMMANDS_ACTIVE, &updateCommandsActive);
    //mogReplaceFunction(PRINT_DEBUG_STRING, &gameLogWrite);
    initializeSrMenuEngine();
    registerPartyCallbacks();
    initializeSrBattleEngine();
    initializeBattleMenu();
    srLogWrite("initialization complete");
    LoadMods();
    finalizeRegistries();
    DispatchBattleMenuSetup();
    MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);

    /* Init RNG */
    srand((unsigned int)time(nullptr));

    /* Launch the lua console */
    //srInitLuaConsole();
}

SISTERRAY_API __declspec(dllexport) void rayInit() {
    /* Early NoCD */
    enableNoCD();

    /* Hook into WinMain (required for Win32 stuff) */
    mogReplaceNop((void*)0x67dbbb, 0x3e);
    mogInsertCall((void*)0x67dbbb, &Init);
}
