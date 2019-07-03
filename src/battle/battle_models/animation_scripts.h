#ifndef ANIMATION_SCRIPTS_H
#define ANIMATION_SCRIPTS_H

#include <SisterRay/SisterRay.h>
#include <map>
#include <unordered_set>
#include "battle_model_utils.h"
#include "model_file_utils.h"
#include "../../sr_named_registry.h"
#include "model_animations.h"

#define BASE_PARTY_SCRIPT_MAX 74
#define BASE_ENEMY_SCRIPT_MAX 32
#define AB_PTR_TABLE_OFFSET 104
typedef std::vector<u8> AnimationScript;

/*Raw buffer size is used for memcpying the relevant information when a model attempts to seek it*/
typedef struct {
    u32 scriptLength;
    AnimationScript animScript;
} SrAnimationScript;

/*This data lives in a registry, and is copied into the right place when the game is allocating models for a given battle
  This is necessary to avoid the game attempting to */
typedef struct {
    ModelABHeader header;
    u16 scriptCount;
    std::map<std::string, SrAnimationScript> modelAnimScripts;
} SrModelScripts;

SrModelScripts createSrModelScripts(SrModelType modelType, const std::string archiveName, void* battleLGPBuffer);

class SrBattleAnimScriptRegistry : public SrNamedResourceRegistry<SrModelScripts, std::string> {
public:
    SrBattleAnimScriptRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer);
    SrBattleAnimScriptRegistry() : SrNamedResourceRegistry<SrModelScripts, std::string>() {}
    u32 getMemoryBufferSize(const std::string& name);
};

AnimationScript animScriptFromBuffer(u8* animScriptStart, u16 animScriptLength, u16* trueScriptLength);
void initAnimationScripts(void* battleLGPBuffer);
void* srInitializeAnimScriptsData(const char* filename, ModelAAHeader* aaHeader);

SISTERRAY_API void animationScriptTrampoline(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2);
static unsigned char newAnimScript[] = { 0xE8, 0xFC, 0x00, 0xED, 0xE6, 0xEA, 0x00, 0x00, 0xEC, 0x00, 0x00, 0xFA, 0xE5, 0xEE };
static unsigned char newAnimScript2[] = { 0xE8, 0xEC, 0x9E };
static unsigned char newAnimScriptEnemies[] = { 0xE8, 0xEC, 0x9E };
//static unsigned char transformationScript[] = { 0xAC, 1, 0xFC, 0, 0xE0, 0xEA, 0xF4, 0x19, 0xF3, 0xD8, 1, 0x29, 0, 0xE2, 0xC5, 0xF3, 0xC9, 0xF4, 1, 0xF3, 0xCA, 0xA2, 0x11, 0xF1 };
static unsigned char transformationScript[] = { 0xAC, 1, 0xFC, 0, 0xE2, 0xC5, 0xF3, 0xC9, 0xF4, 1, 0xF3, 0xCA, 0xA2, 0x11, 0xF1 };

/*We require an array for fetching the new animations by modified command index*/
#pragma pack(push, 1)
typedef struct NewActorAnimScripts {
    u32 pad1; //0x00
    u32 pad2; //0x04
    u32 pad3; //0x08
    u32 ptrToEffectScript1; //0x0C
    u32 ptrToEffectScript2; //0x10
    u32 pad[56]; //0x14
    u32 transformScript; //0x0108
    NewActorAnimScripts();
} NewActorAnimScripts;
#pragma pack(pop)


/*initialize a new array for holding ptrs to new animations*/
static NewActorAnimScripts actorAnimArray[10] = {
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts(),
    NewActorAnimScripts()
};
#endif // !ANIMATION_SCRIPTS_H
