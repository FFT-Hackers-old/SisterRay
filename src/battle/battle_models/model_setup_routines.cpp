#include "model_setup_routines.h"

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
void convertEnemyBDataToPtr() {
    u8* byte_BFD0E0 = (u8*)0xBFD0E0;
    u8* byte_BF2DF8 = (u8*)0xBF2DF8;
    ModelPositionStruct* actorPositionArray = (ModelPositionStruct*)(0xBFD0A0);
    ModelPositionStruct* enemyInitialPositionArray = (ModelPositionStruct*)(0xBF2056);

    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx)
        MODEL_DATA_74_ARRAY[enemyActorIdx].modelDataIndex = 0; //Should investigate this structure

    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx)
        initEnemyModelsFromAB(enemyActorIdx);

    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx) {
        byte_BF2DF8[enemyActorIdx] = UNK_ACTOR_STRUCT_ARRAY[enemyActorIdx].field_1;
        actorPositionArray[enemyActorIdx].xPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].xPosition;
        actorPositionArray[enemyActorIdx].yPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].yPosition;
        actorPositionArray[enemyActorIdx].zPosition = enemyInitialPositionArray[(enemyActorIdx - 4)].zPosition;
        gBigAnimBlock[enemyActorIdx].field_162 = 0;
        gBigAnimBlock[enemyActorIdx].field_15E = 0;
        switch (*byte_BFD0E0) //Position enemies differently based on battle type?
        {
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

void convertsBDataToRealPtr() {
    u8* gCharacterModelCount = (u8*)(0xBF2E0C);
    u8* byte_BFD0E0 = (u8*)0xBFD0E0;
    u8* byte_BF2DF8 = (u8*)0xBF2DF8;
    ModelPositionStruct* partyPositionArray = (ModelPositionStruct*)(0xBFD0A0);
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
                partyPositionArray[partyIdx].xPosition = 0;
                partyPositionArray[partyIdx].yPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].yPosition;
                partyPositionArray[partyIdx].zPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].zPosition;
            }
            sub_42B579();
        }
        case 2: {
            auto relIndex = 0;
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].characterID != 0xFF) {
                    /*This surrounding code is basically an inlined body of sub_42B4AF, with the correct position ptr*/
                    if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_6 & 1) {
                        if (twoPartyAngles[*byte_BFD0E0].angles[relIndex])
                            twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition -= 516;
                        else
                            twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition += 516;
                    }

                    partyPositionArray[partyIdx].xPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].xPosition;
                    partyPositionArray[partyIdx].yPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].yPosition;
                    partyPositionArray[partyIdx].zPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition;


                    gBigAnimBlock[partyIdx].field_18 = twoPartyAngles[*byte_BFD0E0].angles[relIndex];
                    if (*byte_BFD0E0 == 2)
                        gBigAnimBlock[partyIdx].field_160 = 0;
                    else
                        gBigAnimBlock[partyIdx].field_160 = twoPartyAngles[*byte_BFD0E0].angles[relIndex];
                    gBigAnimBlock[partyIdx].field_162 = 0;
                    gBigAnimBlock[partyIdx].field_15E = 0;
                    ++relIndex;
                }
            }
        }
        default: {
            sub_42B4AF();
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                partyPositionArray[partyIdx].xPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].xPosition;
                partyPositionArray[partyIdx].yPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].yPosition;
                partyPositionArray[partyIdx].zPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].zPosition;
            }
            sub_42B579();
        }
    }

    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx)
        byte_BF2DF8[partyIdx] = UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_1;

    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        gBigAnimBlock[partyIdx].restingPosition.xCoordinate = partyPositionArray[partyIdx].xPosition;
        gBigAnimBlock[partyIdx].restingPosition.yCoordinate = partyPositionArray[partyIdx].yPosition;
        gBigAnimBlock[partyIdx].restingPosition.zCoordinate = partyPositionArray[partyIdx].zPosition;
    }
}
