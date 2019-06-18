#include "battle.h"
#include "../impl.h"
#include "../gzip.h"
#include "../party/party_utils.h"
#include <string>
#include <zlib.h>
#include <cstdio>
#include <memory>
#include "../EncodedString.h"

#define BLOCK_SIZE 0x2000
#define SCENE_SIZE 0x1E80

/*Initialize SR registries for enemy/formation data from scene.bin*/
void initFormationsRegistries() {
    srLogWrite("Initializing registries from scene.bin");
    u8* sceneBlock = (u8*)malloc(BLOCK_SIZE);
    SceneLayout currentScene = SceneLayout();
    u16 sceneBlockIndex = 0;
    u16 formationIndex = 0;
    const char* filename = "data/battle/scene.bin";
    FILE* scenePtr = fopen(filename, "rb");
    u32 sceneSize = getFileSize(filename);
    u32 sceneBlockIdx = 0;
    while (sceneSize >= BLOCK_SIZE) {
        readBlock(scenePtr, BLOCK_SIZE, sceneBlock); //We read a scene block into memory, need to determine when last block is reached
        u16 byteSizes[16];
        auto sceneCount = getCompressedFileSizes(sceneBlock, byteSizes); //get the number of scenes in the block and their sizes
        srLogWrite("Loading %i total scenes from block %i", sceneCount, sceneBlockIdx);
        for (auto sceneIndex = 0; sceneIndex < sceneCount; sceneIndex++) {
            auto dwordSceneOffset = (*((u32*)(sceneBlock + 4 * sceneIndex)));
            if (dwordSceneOffset == 0xFFFFFFFF) {
                srLogWrite("end of block reached");
                break;
            }
            srLogWrite("loading scene %i from block %i", sceneIndex, sceneBlockIdx);
            u32 compressedSize = byteSizes[sceneIndex]; //We need to seek the actual end of the last scene
            const u8* compressedScenePtr = sceneBlock + (4 * dwordSceneOffset);
            auto readSize = srGzipInflateCpy(compressedScenePtr, compressedSize, (u8*)&currentScene, sizeof(SceneLayout));
            if (readSize != SCENE_SIZE)
                srLogWrite("ERROR: decompressed scene file is not the right size. Expected size: 7802, actual size %i", readSize);
            populateRegistries(currentScene, &formationIndex); //this mutates formation index and populates formation, enemy data, and enemy ability registries. 
        }
        sceneSize -= BLOCK_SIZE;
        sceneBlockIdx++;
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
void readBlock(FILE* filehandle, u32 blockSize, u8* dst) {
    fread(dst, blockSize, 1, filehandle);
}

/*Populate Enemy, Formation, and Enemy Attack Registries*/
void populateRegistries(const SceneLayout& sceneData, u16* formationIndex) {
    auto sceneIndex = (*formationIndex) / 4;
    std::string sceneName = std::to_string(sceneIndex);
    FormationEnemyIDs srEnemyIDs = FormationEnemyIDs();

    /*Insert the enemies into an enemy registry. They are hashed given a unique ID: Scene Index: Name*/
    for (auto enemyIndex = 0; enemyIndex < 3; enemyIndex++) {
        SrEnemyData srEnemyData = SrEnemyData();
        srEnemyData.enemyData = sceneData.enemyDataArray[enemyIndex];
        srEnemyData.modelID = sceneData.formationModelIDs.EnemyIds[enemyIndex];
        auto enemyName = std::string(EncodedString(srEnemyData.enemyData.enemyName).unicode());
        auto uniqueID = sceneName + enemyName;
        srEnemyIDs.uniqueIDs[enemyIndex] = uniqueID;

        BattleAIData enemyAIData = BattleAIData();
        initializeBattleAIData(&(sceneData.enemyAIData[0]), enemyIndex, enemyAIData);
        srEnemyData.enemyAI = enemyAIData;
        /*Add enemies to the enemy registry*/
        gContext.enemies.add_element(uniqueID, srEnemyData);
        srLogWrite("Enemy:%s added to registry", uniqueID.c_str());
    }

    /*Create a formation referencing the unique enemy IDs*/
    for (auto sceneFormationIndex = 0; sceneFormationIndex < 4; sceneFormationIndex++) {
        FormationData formation = FormationData();
        formation.FormationEnemyIDs = srEnemyIDs; //Probably want to remove this, reconstructing it from the SR unique IDs when a battle is loaded
        formation.formationSetup = sceneData.formationSetupArray[sceneFormationIndex];
        formation.formationCamera = sceneData.formationCameraArray[sceneFormationIndex];
        formation.formationActorDataArray = sceneData.formationActorDataArrays[sceneFormationIndex];
        auto formationName = std::to_string(*formationIndex);
        BattleAIData formationAI = BattleAIData();
        initializeBattleAIData(&(sceneData.formationAIData[0]), sceneFormationIndex, formationAI);
        formation.formationAI = formationAI;
        gContext.formations.add_element(formationName, formation);
        *formationIndex = *formationIndex + 1;
        srLogWrite("Formation:%s added to registry with enemies %s, %s, %s",
            formationName.c_str(),
            formation.FormationEnemyIDs.uniqueIDs[0].c_str(),
            formation.FormationEnemyIDs.uniqueIDs[1].c_str(),
            formation.FormationEnemyIDs.uniqueIDs[2].c_str()
        );
    }

    /*Duduplicate enemy abilities and put them into a registry by attack ID. They can be fetched via the string form of their attack ID*/
    auto& enemyAttacks = sceneData.enemyAttacksArray;
    auto& enemyAttackNames = sceneData.enemyAttackNames;
    auto& enemyAttackIDs = sceneData.enemyAttackIDS;
    for (auto relAttackIndex = 0; relAttackIndex < 32; relAttackIndex++) {
        auto attackID = enemyAttackIDs[relAttackIndex];
        auto stringID = std::string("ETK") + std::to_string(attackID);
        if (attackID == 0xFFFF)
            continue;

        if (!(gContext.attacks.contains(stringID))) { //must implement a method to check if the dictionary contains a key
            auto attackName = EncodedString((const char *)enemyAttackNames[relAttackIndex].name);
            auto attackData = enemyAttacks[relAttackIndex];
            SrAttackData enemyAttack = { attackData, attackID, attackName, ENEMY_ATTACK, ENEMY_ATTACK, EncodedString::from_unicode("") };
            gContext.attacks.add_element(stringID, enemyAttack);
            srLogWrite("Enemy Attack:%s added to registry with name:%s, and ID:%i",
                stringID.c_str(),
                enemyAttack.attackName.unicode(),
                enemyAttack.attackID
            );
        }
    }
    srLogWrite("scene fully registered!");
}
