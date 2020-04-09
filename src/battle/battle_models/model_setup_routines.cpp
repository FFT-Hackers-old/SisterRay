#include "model_setup_routines.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "battle_model_state_interface.h"

typedef void(*PFNSRSUB42B4AF)();
#define sub_42B4AF  ((PFNSRSUB42B4AF)0x42B4AF)

typedef void(*PFNSRSUB42B579)();
#define sub_42B579  ((PFNSRSUB42B579)0x42B579)

typedef void(*PFNSRSUB42BC15)(u8);
#define gameInitEnemiesFromAB   ((PFNSRSUB42BC15)0x42BC15)

typedef void(*PFNSRSUB42B66A)(u8);
#define copyDataFromBFile  ((PFNSRSUB42B66A)0x42B66A)

/*Rewritten because we are now initializing b-data dynamically, so do not want the original
  loops which could only initialize a fixed number of animations scripts*/
void translateEnemyABData() {
    u8* battleType = (u8*)0xBFD0E0;
    u8* byte_BF2DF8 = (u8*)0xBF2DF8;
    u8* enemyCountGlobal = (u8*)0xBF2050;
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    EnemyPosition* enemyInitialPositionArray = (EnemyPosition*)(0xBF2054);

    for (u8 enemyActorIdx = 4; enemyActorIdx < (*enemyCountGlobal) + 4; ++enemyActorIdx) {
        auto actorModelState = getBattleModelState(enemyActorIdx);
        auto actorModel74State = getBattleModelState74(enemyActorIdx);
        actorModel74State->modelDataIndex = 0;
        gameInitEnemiesFromAB(enemyActorIdx);
        byte_BF2DF8[enemyActorIdx] = UNK_ACTOR_STRUCT_ARRAY[enemyActorIdx].field_1;
        actorPositionArray[enemyActorIdx].x = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.x;
        actorPositionArray[enemyActorIdx].y = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.y;
        actorPositionArray[enemyActorIdx].z = enemyInitialPositionArray[(enemyActorIdx - 4)].enemyPosition.z;
        actorModelState->restingZRotation = 0;
        actorModelState->restingXRotation = 0;
        switch (*battleType) {
            case 1: {
                actorModelState->initialYRotation = 2048;
                actorModelState->restingYRotation = 0;
                break;
            }
            case 3:
            case 5:
            case 6:
            case 7: {
                if (actorPositionArray[enemyActorIdx].z >= 0) {
                    actorModelState->restingYRotation = 2048;
                    actorModelState->initialYRotation = 2048;
                }
                else {
                    actorModelState->restingYRotation = 0;
                    actorModelState->initialYRotation = 0;
                }
                break;
            }
            default: {
                if (actorPositionArray[enemyActorIdx].z >= 0) {
                    actorModelState->restingYRotation = 0;
                    actorModelState->initialYRotation = 0;
                }
                else {
                    actorModelState->restingYRotation = 2048;
                    actorModelState->initialYRotation = 2048;
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
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    TwoMemberPositionStruct* twoPartyPositionArray = (TwoMemberPositionStruct*)(0x7C23B0);
    FullPartyPositionStruct* fullPartyPositionArray = (FullPartyPositionStruct*)(0x7C2298);
    UnkTwoPartyStruct* twoPartyAngles = (UnkTwoPartyStruct*)(0x7C24B8);

    *gCharacterModelCount = 0;
    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        auto actorModel74State = getBattleModelState74(partyIdx);
        actorModel74State->modelDataIndex = partyIdx; //The other model data struct, which I do not yet understand
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
                actorPositionArray[partyIdx].x = 0;
                actorPositionArray[partyIdx].y = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].y;
                actorPositionArray[partyIdx].z = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].z;
            }
            sub_42B579();
        }
        case 2: {
            auto relIndex = 0;
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].characterID != 0xFF) {
                    if (UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_6 & 1) {
                        if (twoPartyAngles[*battleType].angles[relIndex])
                            twoPartyPositionArray[*battleType].enemyPosition[relIndex].z -= 516;
                        else
                            twoPartyPositionArray[*battleType].enemyPosition[relIndex].z += 516;
                    }

                    actorPositionArray[partyIdx].x = twoPartyPositionArray[*battleType].enemyPosition[relIndex].x;
                    actorPositionArray[partyIdx].y = twoPartyPositionArray[*battleType].enemyPosition[relIndex].y;
                    actorPositionArray[partyIdx].z = twoPartyPositionArray[*battleType].enemyPosition[relIndex].z;


                    getBattleModelState(partyIdx)->initialYRotation = twoPartyAngles[*battleType].angles[relIndex];
                    if (*battleType == 2)
                        getBattleModelState(partyIdx)->restingYRotation = 0;
                    else
                        getBattleModelState(partyIdx)->restingYRotation = twoPartyAngles[*battleType].angles[relIndex];
                    getBattleModelState(partyIdx)->restingZRotation = 0;
                    getBattleModelState(partyIdx)->restingXRotation = 0;
                    ++relIndex;
                }
            }
        }
        default: {
            sub_42B4AF();
            for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
                actorPositionArray[partyIdx].x = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].x;
                actorPositionArray[partyIdx].y = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].y;
                actorPositionArray[partyIdx].z = fullPartyPositionArray[*battleType].enemyPosition[partyIdx].z;
            }
            sub_42B579();
        }
    }

    for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
        byte_BF2DF8[partyIdx] = UNK_ACTOR_STRUCT_ARRAY[partyIdx].field_1;
        u8* byteViewAnimBlock = (u8*)&(getBattleModelState(partyIdx)->characterID);
        getBattleModelState(partyIdx)->restingPosition.x = actorPositionArray[partyIdx].x;
        getBattleModelState(partyIdx)->restingPosition.y = actorPositionArray[partyIdx].y;
        getBattleModelState(partyIdx)->restingPosition.z = actorPositionArray[partyIdx].z;
    }
}

typedef void(*SRPFNSUB66335F)(u8, u8, u8);
#define sub_66335F    ((SRPFNSUB66335F)0x66335F)

typedef void(*SRPFNSUB68CF75)(u8, LocalWeaponStruct*);
#define zeroWeaponStruct ((SRPFNSUB68CF75)0x68CF75)

typedef i32(*SRPFNSUB66101A)();
#define sub_66101A ((SRPFNSUB66101A)0x66101A)

typedef u8(*SRPFNSUB429343)(i32);
#define sub_429343 ((SRPFNSUB429343)0x429343)

typedef void(*SRPFNSUB684F73)(LocalWeaponStruct*, void*);
#define sub_684F73  ((SRPFNSUB684F73)0x684F73)

typedef void(*SRPFNSUB5E0DDE)(u16, u16, u16, u16, BattleModel*);
#define playWeaponAnimation   ((SRPFNSUB5E0DDE)0x5E0DDE)

typedef Matrix*(*SRPFNSUB67C9BE)(u32, MatrixSet*);
#define getMatrix     ((SRPFNSUB67C9BE)0x67C9BE)

typedef void(*SRPFNSUB6616CF)(GameRotationMatrix*, Matrix*);
#define extractR3AndTranspose    ((SRPFNSUB6616CF)0x6616CF)

/*This function offsets the animation index that is playing and animations the weapon*/
void playCorrectWeaponAnimation(u32 actorIdx) {
    LocalWeaponStruct localWeaponStruct;
    u32* dword_90C9F0 = (u32*)(0x90C9F0);
    u8* byte_BE0E30 = (u8*)(0xBE0E30);

    if (!(*dword_90C9F0))
        return;

    auto& modelState = *getBattleModelState(actorIdx);
    if (!(modelState.field_25 & 4)) {
        sub_66335F(modelState.field_28, modelState.unkActorFlags, modelState.field_2A);
        
        auto actorDataPtr = getGamePartyMember(actorIdx);
        if (!actorDataPtr)
            return;

        BattleModel* modelData = (BattleModel *)modelState.modelDataPtr;
        if (!modelData) {
            return;
        }

        u8 weaponModelID = actorDataPtr->weaponData.weapon_model & 0xF;
        ModelSkeleton* weaponDataPtr = modelData->weaponModels[weaponModelID];
        if (!weaponDataPtr) {
            return;
        }

        LocalWeaponStruct* structPtr = &localWeaponStruct;
        zeroWeaponStruct(0, structPtr);
        //This logic is very strange
        (*((u32*)&structPtr->unkBytes[8])) = sub_66101A();
        structPtr->unkBytes[11] = sub_429343(modelState.field_14);
        //End strange logic
        if (modelState.field_28 || modelState.unkActorFlags || modelState.field_2A) {
            structPtr->unkBytes[0] = 9;
        }
        else {
            structPtr->unkBytes[0] = 10;
        }

        /*Probably modulate opacity based on fraction of HP*/
        /*bool isUltimaWeapon = (!modelState.characterID && weaponModelID == 15);
        if (isUltimaWeapon) {
            if (modelState.field_3F & 8) {
                v6 = 0;
                void * ffContext = GetContext(); 
                sub_660C9A(ffContext);
            }
            else {
                v6 = 1;
            }
            sub_685028(v6, (void(__cdecl *)(u32, u32)(0x4303BE)), weaponDataPtr);
            sub_66C4BC(-4, weaponDataPtr);
            i32 cloudHPRatio = sub_6DC910();
            sub_685028(cloudHPRatio, (void(__cdecl *)(u32, u32))(0x68F758), weaponDataPtr);
        }*/

        sub_684F73(structPtr, weaponDataPtr);
        modelData->unk1 = 0;
        modelData->unk2 = 0;

        std::string& modelName = getSrPartyMember(actorIdx).srPartyMember->modelName;
        u16 weaponOffset = gContext.battleAnimations.getElement(modelName).modelAnimationCount;
        playWeaponAnimation(modelState.setForLimitBreaks, modelState.tableRelativeModelAnimIdx + weaponOffset, modelState.currentPlayingFrame, weaponModelID, modelData);
        Matrix* matrix = getMatrix(0, weaponDataPtr->bonesArray->parts->part->polygonSet->matrixSet);
        extractR3AndTranspose(&(getBattleModelRotationData(actorIdx)->rotationMatrix), matrix);
    }
}
