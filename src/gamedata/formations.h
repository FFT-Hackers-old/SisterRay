#ifndef FORMATIONS_H
#define FORMATIONS_H

#include "../sr_named_registry.h"
#include "../battle/ai_scripts.h"

typedef struct {
    u32 uniqueIDs[3];
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

FormationData& getFormation(u16 modFormationID);
FormationActorData& getFormationActorData(u16 modFormationID, u16 enemyActorIdx);
SISTERRAY_API FormationActorData getFormationActorData(u8 enemyActorIdx);
#endif
