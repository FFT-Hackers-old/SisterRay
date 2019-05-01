#ifndef FORMATIONS_H
#define FORMATIONS_H

#include "../sr_named_registry.h"
#include "ai_scripts.h"

typedef struct {
    u16 EnemyIds[3];
    u16 terminator; //always 0xFF
} FormationEnemies;

typedef struct {
    u16 battleLocation;
    u16 nextFormationID;
    u16 escapeCounter;
    u16 align;
    u16 battleArenaCandidates[4]; //one of the four formation entries listed will be be fetched for the next ID
    u16 battleFlags;
    u8 battleType; //side attack, etc
    u8 initCameraID;
} FormationSetup;

typedef struct {
    u16 xCoordinate;
    u16 yCoordinate;
    u16 zCoordinate;
    u16 xVector;
    u16 yVector;
    u16 zVector;
} CameraData;

typedef struct {
    CameraData mainCamera;
    CameraData animationCams[2];
    u8 padding[12]; //always 0xFFFF -- we might be able to extend/relocate this buffer to enable more camera positions for more dynamic battles
} FormationCamera;

typedef struct {
    u16 enemyID; //This is an index into SrEnemyRegistry -- 0xFFFF by default
    u16 xCoordinate;
    u16 yCoordinate;
    u16 zCoordinate;
    u16 actorRow;
    u16 coverFlags;
    u32 initialStateFlags;
} FormationActorData;

typedef struct {
    FormationEnemies formationEnemyIDs;
    FormationSetup formationSetup;
    FormationCamera formationCamera;
    FormationActorData formationUnits[6];
    BattleAIData formationAI;
} FormationData;

/*The following registries contain formation indexed data used by the battle module*/
class SrFormationRegistry : public SrNamedResourceRegistry<FormationData, std::string> {
public:
    SrFormationRegistry() : SrNamedResourceRegistry<FormationData, std::string>() {}
};

#endif
