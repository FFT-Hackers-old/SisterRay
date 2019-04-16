#ifndef IMPL_H
#define IMPL_H

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
#include "items.h"
#include "materia.h"
#include "weapons.h"
#include "armor.h"
#include "accessory.h"
#include "inventories/inventory.h"
#include "inventories/battle_inventory.h"
#include "inventories/materia_inventory.h"
#include "usable_item_handlers.h"
#include "events/event_bus.h"
#include "string_registry.h"
#include "menus/menu.h"
#include <map>
#include <memory>

/*Game Context holds all the registries which contain
  Resources, in the form of either data or registered callback*/
typedef struct {
    lua_State*                          L;
    FILE*                               logFile;
    SrItemRegistry                      items;
    SrWeaponRegistry                    weapons;
    SrArmorRegistry                     armors;
    SrAccessoryRegistry                 accessories;
    SrMateriaRegistry                   materias;
    std::unique_ptr<SrItemInventory>    inventory;
    std::unique_ptr<SrBattleInventory>  battle_inventory;
    std::unique_ptr<SrMateriaInventory> materia_inventory;
    SrGearViewData                      gearViewData;
    SrItemTypeRegistry                  itemTypeData;
    srOnUseCallbackRegistry             on_use_handlers; /*Registry of function pointers for using items*/
    srNoTargetCallbackRegistry          untargeted_handlers;
    SrOnUseItemDataRegistry             item_on_use_data;
    SrGameStrings                       game_strings;
    MenuRegistry                        menuWidgets;
    EventBus                            eventBus;
} SrContext;

SISTERRAY_GLOBAL SrContext gContext;

SISTERRAY_API void lzssDecompress(char* dst, size_t dstLen, FILE* src);

SISTERRAY_API void srInitLua(void);

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
