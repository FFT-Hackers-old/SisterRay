#include "battle.h"
#include "../impl.h"
#include "../gzip.h"
#include "../party/party_utils.h"
#include <string>
#include <zlib.h>
#include <cstdio>
#include <memory>

#define BLOCK_SIZE 0x2000
#define SCENE_SIZE 0x1E80

typedef u8*(*pfnsub436E15)(u8, u8, u8, i16);
#define createAnimationEvent     ((pfnsub436E15)0x436E15)

typedef i32(*pfnsub437185)(i32, i32, u8, i32);
#define sub_437185               ((pfnsub437185)0x437185)

typedef i32(*pfnsub5D7FE4)(i32, i32, i32);
#define runAIScript              ((pfnsub5D7FE4)0x5D7FE4)

typedef u8(*pfnsub5C8BA1)(void);
#define getRandomNumber          ((pfnsub5C8BA1)0x5C8BA1)

typedef i32(*pfnsub5D9550)(i32, i32, i32);
#define sub_5D9550               ((pfnsub5D9550)0x5D9550)

/*Initialize SR registries for enemy/formation data from scene.bin*/
void initFormationsRegistries() {
    u8* sceneBlock = (u8*)malloc(BLOCK_SIZE);
    SceneLayout currentScene = SceneLayout();
    u16 sceneBlockIndex = 0;
    u16 formationIndex = 0;
    const char* filename = "data/battle/scene.bin";
    u32 sceneSize = getFileSize(filename);
    while (sceneSize >= BLOCK_SIZE) {
        readBlock(filename, BLOCK_SIZE, sceneBlock); //We read a scene block into memory, need to determine when last block is reached
        u16 byteSizes[16];
        auto sceneCount = getCompressedFileSizes(sceneBlock, byteSizes); //get the number of scenes in the block and their sizes
        for (auto sceneIndex = 0; sceneIndex < sceneCount; sceneIndex++) {
            u32 compressedSize = byteSizes[sceneIndex]; //We need to seek the actual end of the last scene
            const u8* compressedScenePtr = sceneBlock + 4 * (*(sceneBlock + 4 * sceneIndex)); //points to the start of the gzipped scene file inside the buffer
            auto readSize = srGzipInflateCpy(compressedScenePtr, compressedSize, (u8*)&currentScene, sizeof(SceneLayout));
            if (readSize != SCENE_SIZE)
                srLogWrite("ERROR: decompressed scene file is not the right size. Expected size: 7802, actual size %i", readSize);
            populateRegistries(currentScene, &formationIndex); //this mutates formation index and populates formation, enemy data, and enemy ability registries. 
        }
        sceneSize -= BLOCK_SIZE;
    }
    free(sceneBlock);
}

/*Returns an array of compressed file sizes*/
u32 getCompressedFileSizes(const u8* sceneBlock, u16 byteSizes[16]) {
    auto sceneCount = 0;
    const u32* blockRelativeOffset = (const u32*)sceneBlock;
    u16 blockSceneIndex = 0;
    while (*blockRelativeOffset != 0xFFFFFFFF) {
        if (*(blockRelativeOffset + 1) == 0xFFFFFFFF) {
            u16 endChar = findEndChar(sceneBlock);
            byteSizes[blockSceneIndex] = endChar - (4 * (*blockRelativeOffset));
        }
        else {
            byteSizes[blockSceneIndex] = 4 * (*(blockRelativeOffset + 1) - *blockRelativeOffset);
        }
        sceneCount++;
        blockRelativeOffset++;
        blockSceneIndex++;
    }
    return sceneCount;
}

/*utility searches a buffer backwards in order to locate the first not 0xFF char for decompressing the final buffer*/
u16 findEndChar(const u8* sceneBlock) {
    const u8* bufferEnd = sceneBlock + BLOCK_SIZE;
    u16 endOffset = BLOCK_SIZE;
    while (bufferEnd > sceneBlock) {
        if (*bufferEnd != 0xFF) {
            return endOffset;
            break;
        }
        bufferEnd--;
        endOffset--;
    }
    srLogWrite("ERROR: final gzip buffer end not found");
}

u32 getFileSize(const char* filename) {
    FILE *fileHandle = NULL;
    fileHandle = fopen(filename, "rb");
    fseek(fileHandle, 0, SEEK_END);
    u32 size = ftell(fileHandle);
    fclose(fileHandle);
    return size;
}

/*Read a scene block into a given buffer*/
void readBlock(const char* filename, u32 blockSize, u8* dst) {
    FILE* scenePtr = fopen(filename, "rb");
    fread(dst, blockSize, 1, scenePtr);
}

/*Populate Enemy, Formation, and Enemy Attack Registries*/
void populateRegistries(const SceneLayout& sceneData, u16* formationIndex) {
    auto sceneIndex = (*formationIndex) / 4;
    std::string sceneName = std::to_string(sceneIndex);
    FormationEnemyIDs uniqueIDs;

    /*Insert the enemies into an enemy registry. They are hashed given a unique ID: Scene Index: Name*/
    for (auto enemyIndex = 0; enemyIndex < 3; enemyIndex++) {
        SrEnemyData enemyData = SrEnemyData();
        enemyData.enemyData = sceneData.enemyDataArray[enemyIndex];
        enemyData.modelID = sceneData.formationModelIDs.EnemyIds[enemyIndex];
        auto enemyName = std::string(enemyData.enemyData.enemyName);
        auto uniqueID = sceneName + enemyName;
        uniqueIDs.uniqueIDs[enemyIndex] = uniqueID;

        BattleAIData enemyAIData = BattleAIData();
        initializeBattleAIData(&(sceneData.enemyAIData[0]), enemyIndex, enemyAIData);
        enemyData.enemyAI = enemyAIData;
        /*Add enemies to the enemy registry*/
        gContext.enemies.add_element(uniqueID, enemyData);
    }

    /*Create a formation referencing the unique enemy IDs*/
    for (auto sceneFormationIndex = 0; sceneFormationIndex < 4; sceneFormationIndex++) {
        FormationData formation = FormationData();
        formation.FormationEnemyIDs = uniqueIDs; //Probably want to remove this, reconstructing it from the SR unique IDs when a battle is loaded
        formation.formationSetup = sceneData.formationSetupArray[sceneFormationIndex];
        formation.formationCamera = sceneData.formationCameraArray[sceneFormationIndex];
        formation.formationActorDataArray = sceneData.formationActorDataArrays[sceneFormationIndex];
        auto formationName = std::to_string(*formationIndex);
        BattleAIData formationAI = BattleAIData();
        initializeBattleAIData(&(sceneData.formationAIData[0]), sceneFormationIndex, formationAI);
        formation.formationAI = formationAI;
        gContext.formations.add_element(formationName, formation);
        *formationIndex = *formationIndex + 1;
    }

    /*Duduplicate enemy abilities and put them into a registry by attack ID. They can be fetched via the string form of their attack ID*/
    auto enemyAttacks = sceneData.enemyAttacksArray;
    auto enemyAttackNames = sceneData.enemyAttackNames;
    auto enemyAttackIDs = sceneData.enemyAttackIDS;
    for (auto attackIndex = 0; attackIndex < 32; attackIndex++) {
        auto attackID = enemyAttackIDs[attackIndex];
        auto stringID = std::to_string(attackID);
        if (!(gContext.enemyAttacks.contains(stringID))) { //must implement a method to check if the dictionary contains a key
            auto attackName = std::string((char*)&(enemyAttackNames[attackIndex].name));
            auto attackData = enemyAttacks[attackIndex];
            EnemyAttack enemyAttack = { attackData, attackID, attackName };
            gContext.enemyAttacks.add_element(stringID, enemyAttack);
        }
    }
}

i16 loadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void)) {
    i16 ret; 
    char v4; 
    int enemyIndex;
    void *v16; 
    i32 sceneSize; 
    i32 sceneIndex; 
    FormationEnemies* formationEnemiesPtr = (FormationEnemies*)(0x9A8748);
    FormationSetup* formationSetupPtr = (FormationSetup*)(0x9A8750);
    FormationCamera* formationCameraPtr = (FormationCamera*)(0x9A8764);
    FormationActorDataArray* formationActorDataPtr = (FormationActorDataArray*)(0x9A8794);
    EnemyData* enemyDataPtr = (EnemyData*)(0x9A8E9C);
    FormationEnemyIDs* sceneAIDataPtr = (FormationEnemyIDs*)(0x9A9CFC);
    u32* dword_C069BC = (u32*)(0xC069BC);
    u32* dword_9A89D0 = (u32*)(0x9A89D0);
    u16* battleFlags = (u16*)(0x9A88A6);
    u16* gFormationType = (u16*)(0x9AAD20);
    u16* gEscapeFlag = (u16*)(0x9AAD0A);
    u16* word_9AAD0E = (u16*)(0x9AAD0E);
    u32* dword_9ACB68 = (u32*)(0x9ACB68);
    u8* battleTypeArray = (u8*)(0x7B76F0);

    ret = *dword_C069BC;  // this code coordinates, along with a2, additional setup for "next battle"
    if (*dword_C069BC) {
        if (*dword_C069BC != 1) {
            if (*dword_C069BC != 2) {
                *dword_C069BC = 0;
                *dword_9A89D0 = 1;
                return ret;
            }
            goto LABEL_11;
        }
    }
    else {
        *dword_C069BC = 1;
    }
    if (!modelAppearCallback) { //This contained a bunch of code which loads stuff from scene. This is not necessary now
        *dword_C069BC = 2;

    LABEL_11:
        auto formationIDstr = std::to_string(formationIndex);
        auto formation = gContext.formations.get_element(std::string(formationIDstr));
        *formationSetupPtr = formation.formationSetup;
        *formationCameraPtr = formation.formationCamera;
        *formationActorDataPtr = formation.formationActorDataArray;

        auto& uniqueEnemyIds = formation.FormationEnemyIDs;
        for (auto enemyIndex = 0; enemyIndex < 3; enemyIndex++) {
            auto currentEnemyData = gContext.enemies.get_element(uniqueEnemyIds.uniqueIDs[enemyIndex]);
            formationEnemiesPtr->EnemyIds[enemyIndex] = currentEnemyData.modelID;
            enemyDataPtr[enemyIndex] = currentEnemyData.enemyData;
            /*While the original game copies AI data here, we parse AI scripts and store them with our registries
              Ergo, we elect to store a battle accessible SR struct which contains unique enemy IDs here*/
            *sceneAIDataPtr = formation.FormationEnemyIDs;
        }

        //Set pre-emptive in some circumstances
        if ((*battleFlags & 4) && (formationSetupPtr->battleFlags & 0x10) && !(formationSetupPtr->battleType))
            formationSetupPtr->battleType = 1;
        *gFormationType = battleTypeArray[formationSetupPtr->battleType];

        if (*battleFlags & 0x40) {                   // Boost stats in battle arena
            formationSetupPtr->battleFlags |= 4u;
            formationSetupPtr->battleLocation = 37;
            formationSetupPtr->initCameraID = (getRandomNumber() & 3) + 96;
            formationSetupPtr->escapeCounter = 1;
            for (enemyIndex = 0; enemyIndex < 3; ++enemyIndex) { // boost all stats for battle arena, need to cap at 255
                enemyDataPtr[enemyIndex].enemyHP = enemyDataPtr[enemyIndex].enemyHP * 2;
                enemyDataPtr[enemyIndex].enemyStrength = (enemyDataPtr[enemyIndex].enemyStrength * 3) / 2;                
                enemyDataPtr[enemyIndex].enemyMagic = (enemyDataPtr[enemyIndex].enemyMagic * 3) / 2;
                enemyDataPtr[enemyIndex].enemyDefense = (enemyDataPtr[enemyIndex].enemyDefense * 3) / 2;
                enemyDataPtr[enemyIndex].enemyMDefense = (enemyDataPtr[enemyIndex].enemyMDefense * 3) / 2;
            }
        }
        else if (*battleFlags & 8) {
            formationSetupPtr->battleFlags &= 0xFFFBu;
        }

        if (!(formationSetupPtr->battleFlags & 4))
            *battleFlags |= 8u;

        *gEscapeFlag = formationSetupPtr->escapeCounter;
        if (*gFormationType == 1 || *gFormationType == 3)
            *gEscapeFlag = 1;
        ret = *gEscapeFlag;
        *word_9AAD0E = *gEscapeFlag;
        *dword_C069BC = 0;
        *dword_9A89D0 = 1;
        return ret;
    }
    ret = modelAppearCallback();
    if (dword_9ACB68) {
        dword_9ACB68 = 0;
        *dword_C069BC = 2;
    }
    return ret;
}

i32 srExecuteAIScript(i32 actorIndex, i32 scriptType, i32 a3) {
    i32 result;
    scriptAnimDataCpy modelDataCpys[10];
    const u8* scriptPtr = nullptr;
    u8 characterScriptIndex = 0xFF; 
    FormationActorDataArray* formationActorDataPtr = (FormationActorDataArray*)(0x9A8794);
    FormationEnemyIDs* sceneAIDataPtr = (FormationEnemyIDs*)(0x9A9CFC);
    u8* linkedScriptArray = (u8*)(0x8FEE38);

    result = 1 << scriptType; 
    u16* unknownPtr = (u16*)(0x9AAD14);
    *unknownPtr |= 1 << scriptType;

    switch (actorIndex) {
        case 0:
        case 1:
        case 2: {
            characterScriptIndex = gUnkPartyDataArray[actorIndex].characterID;
            if (characterScriptIndex != -0xFF && linkedScriptArray[characterScriptIndex] != 0xFF) //this handles script links
                characterScriptIndex = linkedScriptArray[characterScriptIndex];
            // scriptPtr = getAIScriptPtr((i32)&formationAI, characterScriptIndex, scriptType);// characterAI //probably best if we run these AI scripts from an internal registry too
            auto& charAIData = gContext.characters.get_element(getCharacterName(characterScriptIndex)).characterAI;
            scriptPtr = getScriptPtr(charAIData, scriptType);
            break;
        }
        case 3: {
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9: {
            auto formationEnemyID = formationActorDataPtr->formationDatas[actorIndex - 4].enemyID; //fetch the formation relative ID, it's modified from the absolute ID
            auto uniqueID = sceneAIDataPtr->uniqueIDs[formationEnemyID];
            auto& enemyAIData = gContext.enemies.get_element(uniqueID).enemyAI;
            scriptPtr = getScriptPtr(enemyAIData, scriptType);
            break;
        }
        default: {
        }
    }

    if (scriptPtr) {
        //Here we copy the animation index, size scale, and damaged animation of each actor so we can react if they are changed by scripts
        for (auto actorID = 0; actorID < 10; ++actorID) {
            modelDataCpys[actorID].sizeScale = gAiActorVariables[actorID].sizeScale;
            modelDataCpys[actorID].idleAnim = gAiActorVariables[actorID].idleAnimID;
            modelDataCpys[actorID].damagedAnim = gAiActorVariables[actorID].damageAnimID;
        }

        sub_5D9550(actorIndex, a3, 0); //Not sure what this function does yet
        result = runAIScript(actorIndex, (i32)scriptPtr, characterScriptIndex);

        /*If the size scale or animation has changed, we call some functions*/
        for (auto actorID = 0; actorID < 10; ++actorID) {
            if (modelDataCpys[actorID].sizeScale != gAiActorVariables[actorID].sizeScale)
                createAnimationEvent(actorID, 4, gAiActorVariables[actorID].sizeScale, 16);

            if (modelDataCpys[actorID].damagedAnim != gAiActorVariables[actorID].damageAnimID)
                sub_437185(actorID, modelDataCpys[actorID].damagedAnim, gAiActorVariables[actorID].damageAnimID, 0);
        }
    }
    return result;
}
