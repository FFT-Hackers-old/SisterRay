#ifndef MODEL_SETUP_ROUTINES_H
#define MODEL_SETUP_ROUTINES_H

#include <SisterRay/SisterRay.h>

#define FF7_TRANSLATE_ENEMY_BDATA    ((void*)0x42B8D2)
#define FF7_TRANSLATE_PLAYER_BDATA   ((void*)0x42AE73)

#pragma pack(push, 1)
typedef struct {
    u16 xPosition;
    u16 yPosition;
    u16 zPosition;
} ModelPositionStruct;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    ModelPositionStruct enemyPosition[3];
} FullPartyPositionStruct;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    ModelPositionStruct enemyPosition[2];
} TwoMemberPositionStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    ModelPositionStruct enemyPosition;
    u16 unk1;
    u16 unk2;
    u16 unk3;
} EnemyPositionStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 angles[2];
} UnkTwoPartyStruct;
#pragma pack(pop)

void translateEnemyABData();
void translatePlayerABData();

#endif
