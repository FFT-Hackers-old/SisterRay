#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include "game_data_types.h"

#pragma pack(push, 1)
typedef struct {
    char enemyName[32];       //0x00
    u8 enemyLevel;            //0x20
    u8 enemyDex;              //0x21
    u8 enemyLuck;             //0x22
    u8 enemyEvade;            //0x23
    u8 enemyStrength;         //0x24
    u8 enemyDefense;          //0x25
    u8 enemyMagic;            //0x26
    u8 enemyMDefense;         //0x27
    u8 elementTypes[8];       //0x28
    u8 elementModifiers[8];   //0x30
    u8 attackAnimScripts[16]; //0x38
    u16 attackSceneIDs[16];   //0x48
    u16 attackCameraIDs[16];  //0x68
    u8 itemStealDropRates[4]; //0x88
    u16 itemsToStealDrop[4];  //0x8C
    u16 manipAttackIDs[3];    //0x94
    u8 unknownBytes[2];       //0x9A
    u16 enemyMP;              //0x9C
    u16 apAward;              //0x9E
    u16 morphItemID;          //0xA0
    u8 backDamageMultipler;   //0xA2
    u8 align;                 //0xA3
    u32 enemyHP;              //0xA4
    u32 expAward;             //0xA8
    u32 gilAward;             //0xAC
    u32 statusImmunityMask;   //0xB0
    u8 unknown2[4];           //0xB4
} EnemyData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 EnemyIds[3];
    u16 terminator; //always 0xFF
} FormationEnemies;
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 xCoordinate;
    u16 yCoordinate;
    u16 zCoordinate;
    u16 xVector;
    u16 yVector;
    u16 zVector;
} CameraData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    CameraData mainCamera;
    CameraData animationCams[2];
    u8 padding[12]; //always 0xFFFF -- we might be able to extend/relocate this buffer to enable more camera positions for more dynamic battles
} FormationCamera;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 enemyID; //This is an index into SrEnemyRegistry -- 0xFFFF by default
    u16 xCoordinate;
    u16 yCoordinate;
    u16 zCoordinate;
    u16 actorRow;
    u16 coverFlags;
    u32 initialStateFlags;
} FormationActorData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    FormationActorData formationDatas[6];
} FormationActorDataArray;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct{
    u8 name[32];
} AttackNameFixedBuffer;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    FormationEnemies formationModelIDs;
    FormationSetup formationSetupArray[4];
    FormationCamera formationCameraArray[4];
    FormationActorDataArray formationActorDataArrays[4];
    EnemyData enemyDataArray[3];
    AttackData enemyAttacksArray[32];
    u16 enemyAttackIDS[32];
    AttackNameFixedBuffer enemyAttackNames[32];
    u8 formationAIData[512];
    u8 enemyAIData[4096];
} SceneLayout;
#pragma pack(pop)

#endif
