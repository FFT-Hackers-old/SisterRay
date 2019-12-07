#ifndef IMPL_H
#define IMPL_H

#include <windows.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#if defined(__cplusplus)
}
#endif

#include <mog/mog.h>
#include <SisterRay/SisterRay.h>

#include "kernel.h"
#include "gamedata/base_item.h"
#include "gamedata/commands.h"
#include "gamedata/items.h"
#include "gamedata/materia.h"
#include "gamedata/weapons.h"
#include "gamedata/armor.h"
#include "gamedata/accessory.h"
#include "gamedata/attacks.h"
#include "gamedata/formations.h"
#include "gamedata/enemies.h"
#include "gamedata/scene_loader.h"
#include "battle/engine/animation_script_opcode.h"
#include "inventories/inventory.h"
#include "inventories/battle_inventory.h"
#include "inventories/materia_inventory.h"
#include "usable_item_handlers.h"
#include "events/event_bus.h"
#include "string_registry.h"
#include "party/characters.h"
#include "menus/menu.h"
#include "party/party.h"
#include "party/characters.h"
#include "battle/battle_models/animation_registry.h"
#include "battle/battle_models/animation_scripts.h"
#include "battle/battle_models/battle_models.h"
#include <map>
#include <memory>

/*Game Context holds all the registries which contain
  Resources, in the form of either data or registered callback*/
typedef struct {
    lua_State*                          L;
    HWND                                console;
    FILE*                               logFile;

    SrItemTypeRegistry                  itemTypeData;
    SrItemRegistry                      items;
    SrOnUseItemDataRegistry             itemOnUseData;
    srOnUseCallbackRegistry             onUseHandlers; /*Registry of function pointers for using items*/
    srNoTargetCallbackRegistry          untargeted_handlers;

    SrWeaponRegistry                    weapons;
    SrAuxWeaponRegistry                 auxWeapons;
    SrArmorRegistry                     armors;
    SrAuxArmorRegistry                  auxArmors;
    SrAccessoryRegistry                 accessories;
    SrAuxAccessoryRegistry              auxAccessories;
    SrMateriaRegistry                   materias;
    SrAuxMateriaRegistry                auxMaterias;

    SrBattleAnimationRegistry           battleAnimations;
    SrBattleAnimScriptRegistry          battleAnimationScripts;
    SrAnimOpCodeRegistry                animScriptOpcodes;
    SrCommandRegistry                   commands;
    SrAuxCommandRegistry                auxCommands;
    SrAttackRegistry                    attacks;
    SrCharacterRegistry                 characters;
    SrPartyDataRegistry                 party;

    SrFormationRegistry                 formations;
    SrEnemyRegistry                     enemies;

    std::unique_ptr<SrItemInventory>    inventory;
    std::unique_ptr<SrBattleInventory>  battleInventory;
    std::unique_ptr<SrMateriaInventory> materiaInventory;
    SrGearViewData                      gearViewData;

    SrGameStrings                       gameStrings;
    MenuRegistry                        menuWidgets;
    EventBus                            eventBus;
} SrContext;

SISTERRAY_GLOBAL SrContext gContext;

SISTERRAY_API void lzssDecompress(char* dst, size_t dstLen, FILE* src);

SISTERRAY_API void srInitLua(void);
SISTERRAY_API void srInitLuaConsole(void);

SISTERRAY_API void srPatchAddresses(void** patchList, size_t patchCount, void* src, void* dst, size_t offset);
SISTERRAY_API void init_materia(SrKernelStream* stream);
SISTERRAY_API void init_armor(SrKernelStream* stream);
SISTERRAY_API void init_accessory(SrKernelStream* stream);
SISTERRAY_API void initItems(SrKernelStream* stream);
SISTERRAY_API void init_weapon(SrKernelStream* stream);

SISTERRAY_API void initLog(void);
SISTERRAY_API void srLogWrite(const char* format, ...);
SISTERRAY_API void enableNoCD(void);
SISTERRAY_API void initFunctionRegistry(void);
SISTERRAY_API void LoadMods(void);
SISTERRAY_API void LoadKernelFile(void** dst, size_t sectionCount, const char* path);

SISTERRAY_API const void* srLoadFunction(const char* name);
SISTERRAY_API const void* srRegisterFunction(const char* name, const void* func);
SISTERRAY_API const char* srGetGamePath(const char* suffix);

// In-Game internal debugging
void gameLogWrite(const char* str);

/*For testing redirection of animation scripts*/
typedef void (PFNRUNANIMSCRIPT)(u16, u32, u32, u32);
SISTERRAY_GLOBAL PFNRUNANIMSCRIPT* oldRunAnimationScript;

typedef BattleModel*(PFNSETPLAYERDATA)(i32, i32, u8, char*);
SISTERRAY_GLOBAL PFNSETPLAYERDATA* oldSetPlayerModel;

#endif
