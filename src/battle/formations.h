#ifndef FORMATIONS_H
#define FORMATIONS_H

#include "../sr_named_registry.h"
#include "ai_scripts.h"

typedef struct {
    std::string uniqueIDs[3];
} FormationEnemyIDs;

typedef struct {
    FormationEnemyIDs FormationEnemyIDs; //this is the name in gContexts enemy registry
    FormationSetup formationSetup;
    FormationCamera formationCamera;
    FormationActorDataArray formationActorDataArray;
    BattleAIData formationAI;
} FormationData;

/*The following registries contain formation indexed data used by the battle module*/
class SrFormationRegistry : public SrNamedResourceRegistry<FormationData, std::string> {
public:
    SrFormationRegistry() : SrNamedResourceRegistry<FormationData, std::string>() {}
};

#endif
