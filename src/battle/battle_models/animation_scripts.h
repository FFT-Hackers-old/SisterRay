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
    SrNamedResourceRegistry<SrAnimationScript, std::string> modelAnimScripts;
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

#endif // !ANIMATION_SCRIPTS_H
