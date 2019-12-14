#ifndef SCENE_GLOBALS_H
#define SCENE_GLOBALS_H

#include <SisterRay/SisterRay.h>
#include "../gamedata/enemies.h"
#include "../gamedata/formations.h"

FormationEnemies* getInBattleFormationEnemyModels();
FormationSetup* getInBattleFormationSetup();
FormationCamera* getInBattleFormationCamera();
FormationActorDataArray* getInBattleFormationActorDataArray();
FormationActorData* getInBattleFormationActorData(u8 enemyActorIdx);
EnemyData* getInBattleEnemyData(u8 formationRelativeIndex);
u32 getUniqueEnemyID(u8 arrayIdx);
EnemyData* getInBattleActorEnemyData(u8 enemyActorIdx);
const SrEnemyData& getRegistryActorEnemyData(u8 enemyActorIdx);


#endif // !SCENE_GLOBALS_H
