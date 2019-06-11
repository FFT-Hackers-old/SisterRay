#include "model_setup_routines.h"

typedef (*PFNSRSUB42B4AF)();
#define sub_42B4AF  ((PFNSRSUB42B4AF)0x42B4AF)

typedef (*PFNSRSUB42B579)();
#define sub_42B579  ((PFNSRSUB42B579)0x42B579)

/*Rewritten because we are now initializing b-data dynamically, so do not want the original
  loops which could only initialize a fixed number of animations*/
void convertEnemyBDataToPtr() {
    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx)
        modelDataIndex[116 * enemyActorIdx] = 0; //Should investigate this structure

    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx)
        sub_42BC15(enemyActorIdx);

    for (auto enemyActorIdx = 4; enemyActorIdx < enemyCountGlobal + 4; ++enemyActorIdx) {
        byte_BF2DF8[enemyActorIdx] = byte_9A87F5[16 * enemyActorIdx];
        word_BFD0A0[3 * enemyActorIdx] = word_BF2056[6 * (enemyActorIdx - 4)];
        word_BFD0A2[3 * enemyActorIdx] = word_BF2058[6 * (enemyActorIdx - 4)];
        word_BFD0A4[3 * enemyActorIdx] = word_BF205A[6 * (enemyActorIdx - 4)];
        gBigAnimBlock[enemyActorIdx].field_162 = 0;
        gBigAnimBlock[enemyActorIdx].field_15E = 0;
        switch (byte_BFD0E0)
        {
            case 1:
                gBigAnimBlock[enemyActorIdx].field_18 = 2048;
                gBigAnimBlock[enemyActorIdx].field_160 = 0;
                break;
            case 3:
            case 5:
            case 6:
            case 7:
                if (word_BFD0A4[3 * enemyActorIdx] >= 0) {
                    gBigAnimBlock[enemyActorIdx].field_160 = 2048;
                    gBigAnimBlock[enemyActorIdx].field_18 = 2048;
                }
                else {
                    gBigAnimBlock[enemyActorIdx].field_160 = 0;
                    gBigAnimBlock[enemyActorIdx].field_18 = 0;
                }
                break;
            default:
                if (word_BFD0A4[3 * enemyActorIdx] >= 0) {
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

void convertsBDataToRealPtr() {
    u8* gCharacterModelCount = (u8*)(0xBF2E0C);
    u8* byte_BFD0E0 = (u8*)0xBFD0E0;
    ModelPositionStruct* partyPositionArray = (ModelPositionStruct*)(0xBFD0A0);
    TwoMemberPositionStruct* twoPartyPositionArray = (TwoMemberPositionStruct*)(0x7C23B0);
    FullPartyPositionStruct* fullPartyPositionArray = (FullPartyPositionStruct*)(0x7C2298);

    *gCharacterModelCount = 0;
    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        modelDataIndex[116 * partyIdx] = partyIdx;
        if (characterID[16 * partyIdx] != 0xFF) {
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
                partyPositionArray[partyIdx].zPosition = fullPartyPositionArray[*byte_BFD0E0].partyPostions[partyIdx].zPosition
            }
            sub_42B579();
        }
        case 2: {
            auto relIndex = 0;
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].characterID != 0xFF) {
                    /*This surrounding code is basically an inlined body of sub_42B4AF, with the correct position ptr*/
                    if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_7 & 1) {
                        if (word_7C24B8[2 * (*byte_BFD0E0) + relIndex])
                            twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition -= 516;
                        else
                            twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition += 516;
                    }

                    partyPositionArray[partyIdx].xPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].xPosition;
                    partyPositionArray[partyIdx].yPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].yPosition;
                    partyPositionArray[partyIdx].zPosition = twoPartyPositionArray[*byte_BFD0E0].partyPostions[relIndex].zPosition;


                    if (characterID[16 * partyIdx] != -1) {
                        gBigAnimBlock[partyIdx].field_18 = word_7C24B8[2 * (*byte_BFD0E0) + relIndex];
                        if (*byte_BFD0E0 == 2)
                            gBigAnimBlock[partyIdx].field_160 = 0;
                        else
                            gBigAnimBlock[partyIdx].field_160 = word_7C24B8[2 * (*byte_BFD0E0) + relIndex];
                        gBigAnimBlock[partyIdx].field_162 = 0;
                        gBigAnimBlock[partyIdx].field_15E = 0;
                        ++relIndex;
                    }
                    relIndex++;
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
        byte_BF2DF8[partyIdx] = byte_9A87F5[16 * partyIdx];

    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        gBigAnimBlock[partyIdx].restingPosition.xCoordinate = partyPositionArray[partyIdx].xPosition;
        gBigAnimBlock[partyIdx].restingPosition.yCoordinate = partyPositionArray[partyIdx].yPosition;
        gBigAnimBlock[partyIdx].restingPosition.zCoordinate = partyPositionArray[partyIdx].zPosition;
    }
}
