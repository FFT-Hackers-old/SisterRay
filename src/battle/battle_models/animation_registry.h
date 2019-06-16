#ifndef ANIMATION_REGISTRY
#define ANIMATION_REGISTRY

#include "model_animations.h"
#include <unordered_set>
#include <map>
#include <string>
#include "../../sr_named_registry.h"

#define BASE_WEAPON_OFFSET (u16)0x34 //This is the offset between a model animation and its weapon animation for player models
//Indexes 0x2C - 0x34 are reserve for limits, but the data doesn't appear to be in the animation file

/*These are player data model names, which possess weapons*/
static const std::vector<std::string> characterModelNames = {
    std::string("CLOUD.DAT"),
    std::string("BARRETT.DAT"),
    std::string("TIFA.DAT"),
    std::string("EARITH.DAT"),
    std::string("RED13.DAT"),
    std::string("YUFI.DAT"),
    std::string("KETCY.DAT"),
    std::string("VINSENT.DAT"),
    std::string("CID1.DAT"),
    std::string("SEFIROS.DAT"),
    std::string("BARRETT2.DAT"),
    std::string("BARRETT3.DAT"),
    std::string("BARRETT4.DAT"),
    std::string("VINSENT2.DAT"),
    std::string("VINSENT3.DAT"),
    std::string("HICLOUD.DAT")
};

static const std::vector<std::string> specialModelNames = {
    std::string("FROG.DAT"),
    std::string("GALL.DAT"),
    std::string("DEATHGIG.DAT"),
    std::string("HELLMASK.DAT"),
    std::string("CHAOS.DAT")
};

/*Raw buffer size is used for memcpying the relevant information when a model attempts to seek it*/
typedef struct {
    u32 rawBufferSize; //This is used when memcopying to the buffers actually used in battle
    BattleAnimation* animationData;
} SrAnimation;

/*This data lives in a registry, and is copied into the right place when the game is allocating models for a given battle
  This is necessary to avoid the game attempting to */
typedef struct SrModelAnimations {
    SrModelType type;
    u16 modelAnimationCount;
    u16 weaponsAnimationCount;
    u16 totalAnimationCount;
    std::map<std::string, SrAnimation> modelAnimations;
    std::map<std::string, SrAnimation> weaponAnimations;
} SrModelAnimations;

SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon, void* battleLGPBuffer);

/*Registry which holds animation data in memory for the various models, player data models will be keyed by named,
  Enemies will be keyed by their their model index number*/
class SrBattleAnimationRegistry : public SrNamedResourceRegistry<SrModelAnimations, std::string> {
public:
    SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer); 
    SrBattleAnimationRegistry() : SrNamedResourceRegistry<SrModelAnimations, std::string>() {}
};

const std::string assembleAnimKey(u16 idx);
void initAnimations(void* battleLGPBuffer);
void srInitializeAnimationsTable(void** animationDataTable, u16 tableSize, const char* filename, ModelAAHeader* aaHeader);
#endif // !1
