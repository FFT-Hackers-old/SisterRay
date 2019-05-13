#ifndef ENEMIES_H
#define ENEMIES_H

#include "../sr_named_registry.h"
#include "ai_scripts.h"

typedef struct {
    EnemyData enemyData;
    u16 modelID;
    BattleAIData enemyAI;
} SrEnemyData;

typedef struct {
    AttackData attackData;
    u16 attackID;
    std::string attackName;
} EnemyAttack;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrEnemyRegistry : public SrNamedResourceRegistry<SrEnemyData, std::string> {
public:
    SrEnemyRegistry() : SrNamedResourceRegistry<SrEnemyData, std::string>() {}
};

class SrEnemyAttackRegistry : public SrNamedResourceRegistry<EnemyAttack, std::string> {
public:
    SrEnemyAttackRegistry() : SrNamedResourceRegistry<EnemyAttack, std::string>() {}
};


#endif
