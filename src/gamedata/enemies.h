#ifndef ENEMIES_H
#define ENEMIES_H

#include "../sr_named_registry.h"
#include "../battle/ai_scripts.h"
#include "../EncodedString.h"
#include <unordered_set>

typedef struct {
    EnemyData enemyData;
    u16 modelID;
    BattleAIData enemyAI;
} SrEnemyData;

typedef struct {
    AttackData attackData;
    u16 attackID;
    EncodedString attackName;
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

std::string assembleEnemyDataKey(std::string uniqueID);

#endif
