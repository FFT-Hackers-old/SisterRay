#include "model_setup_routines.h"
#include "../../impl.h"

typedef void(*PFNSRSUB42B4AF)();
#define sub_42B4AF  ((PFNSRSUB42B4AF)0x42B4AF)

typedef void(*PFNSRSUB42B579)();
#define sub_42B579  ((PFNSRSUB42B579)0x42B579)

typedef void(*PFNSRSUB42BC15)(u8);
#define initEnemyModelsFromAB   ((PFNSRSUB42BC15)0x42BC15)

typedef void(*PFNSRSUB42B66A)(u8);
#define copyDataFromBFile  ((PFNSRSUB42B66A)0x42B66A)

/*Rewritten because we are now initializing b-data dynamically, so do not want the original
  loops which could only initialize a fixed number of animations scripts*/
void translateEnemyABData() {
    u8* battleType = (u8*)0xBFD0E0;
    u8* byte_BF2DF8 = (u8*)0xBF2DF8;
    u8* enemyCountGlobal = (u8*)0xBF2050;
    ModelPositionStruct* actorPositionArray = (ModelPositionStruct*)(0xBFD0A0);
    EnemyPositionStruct* enemyInitialPositionArray = (EnemyPositionStruct*)(0xBF2056);

    for (u16 enemyActorIdx = 4; enemyActorIdx < (*enemyCountGlobal) + 4; ++enemyActorIdx) {
        MODEL_DATA_74_ARRAY[enemyActorIdx].modelDataIndex = 0;
        initEnemyModelsFromAB(enemyActorIdx);
        byte_BF2DF8[enemyActorIdx] = UNK_ACTOR_STRUCT_ARRAY[enemyActorIdx].field_1;
        srLogWrite("SETTING POSITION OF ENEMY AT INDEX %i, x:%i, y:%i, z:%i", enemyActorIdx,
            enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.xPosition,
            enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.yPosition,
            enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.zPosition
        );
        actorPositionArray[enemyActorIdx].xPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.xPosition;
        actorPositionArray[enemyActorIdx].yPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.yPosition;
        actorPositionArray[enemyActorIdx].zPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.zPosition;
        gBigAnimBlock[enemyActorIdx].field_162 = 0;
        gBigAnimBlock[enemyActorIdx].field_15E = 0;
        switch (*battleType) {
            case 1:
                gBigAnimBlock[enemyActorIdx].field_18 = 2048;
                gBigAnimBlock[enemyActorIdx].field_160 = 0;
                break;
            case 3:
            case 5:
            case 6:
            case 7: {
                if (actorPositionArray[enemyActorIdx].zPosition >= 0) {
                    gBigAnimBlock[enemyActorIdx].field_160 = 2048;
                    gBigAnimBlock[enemyActorIdx].field_18 = 2048;
                }
                else {
                    gBigAnimBlock[enemyActorIdx].field_160 = 0;
                    gBigAnimBlock[enemyActorIdx].field_18 = 0;
                }
                break;
            }
            default: {
                if (actorPositionArray[enemyActorIdx].zPosition >= 0) {
                    gBigAnimBlock[enemyActorIdx].field_160 = 0;
                    gBigAnimBlock[enemyActorIdx].field_18 = 0;
                }
                else
                {
                    gBigAnimBlock[enemyActorIdx].field_160 = 2048;
                    gBigAnimBlock[enemyActorIdx].field_18 = 2048;
                }
                break;
            }
        }
    }
}

void translatePlayerABData() {
    u8* gCharacterModelCount = (u8*)(0xBF2E0C);
    u8* battleType = (u8*)0xBFD0E0;
    u8* byte_BF2DF8 = (u8*)0xBF2DF8;
    ModelPositionStruct* actorPositionArray = (ModelPositionStruct*)(0xBFD0A0);
    TwoMemberPositionStruct* twoPartyPositionArray = (TwoMemberPositionStruct*)(0x7C23B0);
    FullPartyPositionStruct* fullPartyPositionArray = (FullPartyPositionStruct*)(0x7C2298);
    UnkTwoPartyStruct* twoPartyAngles = (UnkTwoPartyStruct*)(0x7C24B8);

    *gCharacterModelCount = 0;
    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        MODEL_DATA_74_ARRAY[partyIdx].modelDataIndex = partyIdx; //The other model data struct, which I do not yet understand
        if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].characterID != 0xFF) {
            copyDataFromBFile(partyIdx);
            *gCharacterModelCount = *gCharacterModelCount + 1;
        }
    }

    /*If there is one party member*/
    switch (*gCharacterModelCount) {
        case 1: {
            sub_42B4AF();
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                actorPositionArray[partyIdx].xPosition = 0;
                actorPositionArray[partyIdx].yPosition = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].yPosition;
                actorPositionArray[partyIdx].zPosition = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].zPosition;
            }
            sub_42B579();
        }
        case 2: {
            auto relIndex = 0;
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].characterID != 0xFF) {
                    /*This surrounding code is basically an inlined body of sub_42B4AF, with the correct position ptr*/
                    if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_6 & 1) {
                        if (twoPartyAngles[*battleType].angles[relIndex])
                            twoPartyPositionArray[*battleType].enemyPosition[relIndex].zPosition -= 516;
                        else
                            twoPartyPositionArray[*battleType].enemyPosition[relIndex].zPosition += 516;
                    }

                    actorPositionArray[partyIdx].xPosition = twoPartyPositionArray[*battleType].enemyPosition[relIndex].xPosition;
                    actorPositionArray[partyIdx].yPosition = twoPartyPositionArray[*battleType].enemyPosition[relIndex].yPosition;
                    actorPositionArray[partyIdx].zPosition = twoPartyPositionArray[*battleType].enemyPosition[relIndex].zPosition;


                    gBigAnimBlock[partyIdx].field_18 = twoPartyAngles[*battleType].angles[relIndex];
                    if (*battleType == 2)
                        gBigAnimBlock[partyIdx].field_160 = 0;
                    else
                        gBigAnimBlock[partyIdx].field_160 = twoPartyAngles[*battleType].angles[relIndex];
                    gBigAnimBlock[partyIdx].field_162 = 0;
                    gBigAnimBlock[partyIdx].field_15E = 0;
                    ++relIndex;
                }
            }
        }
        default: {
            sub_42B4AF();
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                actorPositionArray[partyIdx].xPosition = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].xPosition;
                actorPositionArray[partyIdx].yPosition = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].yPosition;
                actorPositionArray[partyIdx].zPosition = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].zPosition;
            }
            sub_42B579();
        }
    }

    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx)
        byte_BF2DF8[partyIdx] = UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_1;


    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        u8* byteViewAnimBlock = (u8*)&(gBigAnimBlock[partyIdx].actorID);
        gBigAnimBlock[partyIdx].restingPosition.xCoordinate = actorPositionArray[partyIdx].xPosition;
        gBigAnimBlock[partyIdx].restingPosition.yCoordinate = actorPositionArray[partyIdx].yPosition;
        gBigAnimBlock[partyIdx].restingPosition.zCoordinate = actorPositionArray[partyIdx].zPosition;
        *(byteViewAnimBlock + (6982 * partyIdx) + 0x166) = actorPositionArray[partyIdx].xPosition;
        *(byteViewAnimBlock + (6982 * partyIdx) + 0x168) = actorPositionArray[partyIdx].yPosition;
        *(byteViewAnimBlock + (6982 * partyIdx) + 0x16A) = actorPositionArray[partyIdx].zPosition;
        srLogWrite("offset of resting position structure in battle model state: %p", (&(gBigAnimBlock[partyIdx].restingPosition.xCoordinate) - &(gBigAnimBlock[partyIdx].actorID)));
        srLogWrite("positioning party member %i at location %i, %i, %i", partyIdx,
            gBigAnimBlock[partyIdx].restingPosition.xCoordinate,
            gBigAnimBlock[partyIdx].restingPosition.yCoordinate,
            gBigAnimBlock[partyIdx].restingPosition.zCoordinate
        );
    }
}
