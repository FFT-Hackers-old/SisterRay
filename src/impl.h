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
#include "gamedata/items.h"
#include "gamedata/materia.h"
#include "gamedata/weapons.h"
#include "gamedata/armor.h"
#include "gamedata/accessory.h"
#include "gamedata/attacks.h"
#include "battle/formations.h"
#include "battle/enemies.h"
#include "inventories/inventory.h"
#include "inventories/battle_inventory.h"
#include "inventories/materia_inventory.h"
#include "usable_item_handlers.h"
#include "events/event_bus.h"
#include "string_registry.h"
#include "party/characters.h"
#include "menus/menu.h"
#include "party/party.h"
#include "party/character.h"
#include <map>
#include <memory>

/*Game Context holds all the registries which contain
  Resources, in the form of either data or registered callback*/
typedef struct {
    lua_State*                          L;
    HWND                                console;
    FILE*                               logFile;
    SrItemRegistry                      items;
    SrWeaponRegistry                    weapons;
    SrArmorRegistry                     armors;
    SrAccessoryRegistry                 accessories;
    SrMateriaRegistry                   materias;
    std::unique_ptr<SrItemInventory>    inventory;
    std::unique_ptr<SrBattleInventory>  battleInventory;
    std::unique_ptr<SrMateriaInventory> materiaInventory;
    SrGearViewData                      gearViewData;
    SrItemTypeRegistry                  itemTypeData;
    srOnUseCallbackRegistry             onUseHandlers; /*Registry of function pointers for using items*/
    srNoTargetCallbackRegistry          untargeted_handlers;
    SrOnUseItemDataRegistry             itemOnUseData;
    SrCharacterRegistry                 characters;
    SrPartyDataRegistry                 party;
    SrFormationRegistry                 formations;
    SrEnemyRegistry                     enemies;
    SrEnemyAttackRegistry               enemyAttacks;
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

#endif
