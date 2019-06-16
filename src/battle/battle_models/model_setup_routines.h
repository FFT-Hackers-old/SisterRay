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
    ModelPositionStruct partyPostions[3];
} FullPartyPositionStruct;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    ModelPositionStruct partyPostions[2];
} TwoMemberPositionStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 angles[2];
} UnkTwoPartyStruct;
#pragma pack(pop)

void translateEnemyABData();
void translatePlayerABData();

#endif
