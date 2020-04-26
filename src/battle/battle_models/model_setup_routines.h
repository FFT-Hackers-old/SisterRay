#ifndef MODEL_SETUP_ROUTINES_H
#define MODEL_SETUP_ROUTINES_H

#include <SisterRay/SisterRay.h>
#include "battle_models.h"

#define FF7_TRANSLATE_ENEMY_BDATA    ((void*)0x42B8D2)
#define FF7_TRANSLATE_PLAYER_BDATA   ((void*)0x42AE73)

typedef void(*PFNSRSUB42BC15)(u8);
#define gameInitEnemiesFromAB   ((PFNSRSUB42BC15)0x42BC15)


typedef void(*PFNSRSUB42B66A)(u8);
#define copyDataFromBFile  ((PFNSRSUB42B66A)0x42B66A)



#pragma pack(push, 1)
typedef struct {
    R3PointWord enemyPosition[3];
} FullPartyPositionStruct;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    R3PointWord enemyPosition[2];
} TwoMemberPositionStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 modelArrayIdx;
    R3PointWord enemyPosition;
    u16 colorMask;
    u16 unk3;
} EnemyPosition;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 angles[2];
} UnkTwoPartyStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u8 unkBytes[0x44];
} LocalWeaponStruct;
#pragma pack(pop)


void translateEnemyABData();
void translatePlayerABData();
void playCorrectWeaponAnimation(u32 actorIdx);

#endif
