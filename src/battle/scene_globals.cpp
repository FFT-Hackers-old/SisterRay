#include "scene_globals.h"
#include "../impl.h"

#define FF7_FORMATION_ENEMIES           ((FormationEnemies*)(0x9A8748));
#define FF7_FORMATION_SETUP             ((FormationSetup*)(0x9A8750));
#define FF7_FORMATION_CAMERA            ((FormationCamera*)(0x9A8764));
#define FF7_FORMATION_ACTOR_DATA        ((FormationActorDataArray*)(0x9A8794));

FormationEnemies* getInBattleFormationEnemyModels() {
    return FF7_FORMATION_ENEMIES;
}

FormationSetup* getInBattleFormationSetup() {
    return FF7_FORMATION_SETUP;
}

FormationCamera* getInBattleFormationCamera() {
    return FF7_FORMATION_CAMERA;
}

FormationActorDataArray* getInBattleFormationActorDataArray() {
    return FF7_FORMATION_ACTOR_DATA;
}

FormationActorData* getInBattleFormationActorData(u8 enemyActorIdx) {
    FormationActorDataArray* formationActorDataPtr = getInBattleFormationActorDataArray();
    return &(formationActorDataPtr->formationDatas[enemyActorIdx]);
}

EnemyData* getInBattleEnemyData(u8 formationRelativeIndex) {
    EnemyData* enemyDataPtr = (EnemyData*)(0x9A8E9C);
    return &(enemyDataPtr[formationRelativeIndex]);
}

std::string getUniqueEnemyID(u8 arrayIdx) {
    if (arrayIdx > 3) {
        return nullptr;
    }
    FormationEnemyIDs* sceneAIDataPtr = (FormationEnemyIDs*)(0x9A9CFC);
    return sceneAIDataPtr->uniqueIDs[arrayIdx];
}


//Use in battle to fetch the in battle copy of enemy data, which you can mutate
EnemyData* getInBattleActorEnemyData(u8 enemyActorIdx) {
    auto sceneRelativeID = getInBattleFormationActorData(enemyActorIdx)->enemyID;
    return getInBattleEnemyData(sceneRelativeID);
}

//Use in battle to fetch the registry Data for an enemy
const SrEnemyData& getRegistryActorEnemyData(u8 enemyActorIdx) {
    auto sceneRelativeID = getInBattleFormationActorData(enemyActorIdx)->enemyID;
    auto uniqueID = getUniqueEnemyID(sceneRelativeID);
    return gContext.enemies.get_element(uniqueID);
}
