#ifndef ENEMIES_H
#define ENEMIES_H

#include "../sr_named_registry.h"
#include "../battle/ai_script/ai_scripts.h"
#include "../EncodedString.h"
#include "../gamedata/battle_stats.h"
#include "../gamedata/stat_names.h"
#include <unordered_set>
#include <vector>

typedef struct {
    u16 itemID;
    u16 stealChance;
    bool alreadyStolen;
} StealItem;

struct SrEnemyData_ {
    EnemyData enemyData;
    u16 modelID;
    BattleAIData enemyAI;
    std::unordered_map<std::string, SrBoostedStat> enemyStats;
    std::vector<StealItem> toSteal;
    bool canMorph;
    u16 morphItem;
    bool isSceneEnemy;
    u16 gameSceneIndex;
    u8 sceneRelativeIdx;
};

typedef struct {
    AttackData attackData;
    u16 attackID;
    EncodedString name;
} EnemyAttack;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrEnemyRegistry : public SrNamedResourceRegistry<SrEnemyData, std::string> {
public:
    SrEnemyRegistry() : SrNamedResourceRegistry<SrEnemyData, std::string>() {}
    std::unordered_set<u16> getUniqueModelIDs();
};

class SrEnemyAttackRegistry : public SrNamedResourceRegistry<EnemyAttack, std::string> {
public:
    SrEnemyAttackRegistry() : SrNamedResourceRegistry<EnemyAttack, std::string>() {}
};


void finalizeEnemies();
std::string assembleEnemyDataKey(std::string uniqueID);

#endif
