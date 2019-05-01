#ifndef ENEMIES_H
#define ENEMIES_H

#include "../sr_named_registry.h"
#include "ai_scripts.h"

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrEnemyRegistry : public SrNamedResourceRegistry<EnemyData, std::string> {
public:
    SrEnemyRegistry() : SrNamedResourceRegistry<EnemyData, std::string>() {}
};

class SrEnemyAIRegistry: public SrNamedResourceRegistry<BattleAIData, std::string> {
public:
    SrEnemyAIRegistry() : SrNamedResourceRegistry<BattleAIData, std::string>() {}
};

#endif
