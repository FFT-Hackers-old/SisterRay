#ifndef MODEL_SETUP_ROUTINES_H
#define MODEL_SETUP_ROUTINES_H

#include <SisterRay/SisterRay.h>

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

#endif