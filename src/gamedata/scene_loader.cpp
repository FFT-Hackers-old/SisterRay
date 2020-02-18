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
    u32 uniqueEnemyIdx = 0;
    while (sceneSize >= BLOCK_SIZE) {
        readBlock(scenePtr, BLOCK_SIZE, sceneBlock);
        u16 byteSizes[16];
        auto sceneCount = getCompressedFileSizes(sceneBlock, byteSizes);
        srLogWrite("Loading %i total scenes from block %i", sceneCount, sceneBlockIdx);
        for (auto sceneIndex = 0; sceneIndex < sceneCount; sceneIndex++) {
            auto dwordSceneOffset = (*((u32*)(sceneBlock + 4 * sceneIndex)));
            if (dwordSceneOffset == 0xFFFFFFFF) {
                break;
            }
            srLogWrite("loading scene %i from block %i", sceneIndex, sceneBlockIdx);
            u32 compressedSize = byteSizes[sceneIndex]; 
            const u8* compressedScenePtr = sceneBlock + (4 * dwordSceneOffset);
            auto readSize = srGzipInflateCpy(compressedScenePtr, compressedSize, (u8*)&currentScene, sizeof(SceneLayout));
            if (readSize != SCENE_SIZE)
                srLogWrite("ERROR: decompressed scene file is not the right size. Expected size: 7802, actual size %i", readSize);
            populateRegistries(currentScene, &formationIndex, &uniqueEnemyIdx); 
        }
        sceneSize -= BLOCK_SIZE;
        sceneBlockIdx++;
    }
    free(sceneBlock);
    auto& command = gContext.commands.getElement(assembleGDataKey(CMD_ENEMY_ACTION));
    command.actionCount = command.commandActions.size();
    srLogWrite("FINISHED REGISTERING ENEMY ACTIONS: registered %d enemy attacks", getCommand(CMD_ENEMY_ACTION).actionCount);
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
void populateRegistries(const SceneLayout& sceneData, u16* formationIndex, u32* uniqueIdx) {
    auto sceneIndex = (*formationIndex) / 4;
    std::string sceneName = std::to_string(sceneIndex);
    FormationEnemyIDs srEnemyIDs = FormationEnemyIDs();

    /*Insert the enemies into an enemy registry. They are hashed given a unique ID: Scene Index: Name*/
    for (auto formationRelativeEnemyIdx = 0; formationRelativeEnemyIdx < 3; formationRelativeEnemyIdx++) {
        SrEnemyData srEnemyData = SrEnemyData();
        srEnemyData.enemyData = sceneData.enemyDataArray[formationRelativeEnemyIdx];
        srEnemyData.modelID = sceneData.formationModelIDs.EnemyIds[formationRelativeEnemyIdx];
        auto registryName = assembleGDataKey(*uniqueIdx);
        srEnemyIDs.uniqueIDs[formationRelativeEnemyIdx] = *uniqueIdx;
        *uniqueIdx += 1;

        BattleAIData enemyAIData = BattleAIData();
        initializeBattleAIData(&(sceneData.enemyAIData[0]), formationRelativeEnemyIdx, enemyAIData);
        srEnemyData.enemyAI = enemyAIData;
        calculateEnemyStats(srEnemyData);
        /*Add enemies to the enemy registry*/
        gContext.enemies.addElement(registryName, srEnemyData);
        srLogWrite("Enemy:%s added to registry with unique idx %d", registryName.c_str(), *uniqueIdx);
    }

    /*Create a formation referencing the unique enemy IDs*/
    for (auto sceneFormationIndex = 0; sceneFormationIndex < 4; sceneFormationIndex++) {
        FormationData formation = FormationData();
        formation.FormationEnemyIDs = srEnemyIDs; 
        formation.formationSetup = sceneData.formationSetupArray[sceneFormationIndex];
        formation.formationCamera = sceneData.formationCameraArray[sceneFormationIndex];
        formation.formationActorDataArray = sceneData.formationActorDataArrays[sceneFormationIndex];
        BattleAIData formationAI = BattleAIData();
        initializeBattleAIData(&(sceneData.formationAIData[0]), sceneFormationIndex, formationAI);
        formation.formationAI = formationAI;
        gContext.formations.addElement(assembleGDataKey(*formationIndex), formation);
        *formationIndex = *formationIndex + 1;
        srLogWrite("Formation:%s added to registry with enemies %d, %d, %d",
            assembleGDataKey(*formationIndex).c_str(),
            formation.FormationEnemyIDs.uniqueIDs[0],
            formation.FormationEnemyIDs.uniqueIDs[1],
            formation.FormationEnemyIDs.uniqueIDs[2]
        );
    }

    /*Duduplicate enemy abilities and put them into a registry by attack ID. They can be fetched via the string form of their attack ID*/
    auto& enemyAttacks = sceneData.enemyAttacksArray;
    auto& enemyAttackNames = sceneData.enemyAttackNames;
    auto& enemyAttackIDs = sceneData.enemyAttackIDS;
    for (auto relAttackIndex = 0; relAttackIndex < 32; relAttackIndex++) {
        auto attackID = enemyAttackIDs[relAttackIndex];
        auto stringID = assembleGDataKey(attackID);
        if (attackID == 0xFFFF)
            continue;

        if (!(gContext.attacks.contains(stringID))) { //must implement a method to check if the dictionary contains a key
            auto attackName = EncodedString((const char *)enemyAttackNames[relAttackIndex].name);
            auto attackData = enemyAttacks[relAttackIndex];
            SrAttack enemyAttack = { attackData, attackID, attackName, EncodedString::from_unicode(""), ENEMY_ATTACK,  };
            gContext.attacks.addElement(stringID, enemyAttack);
            /*srLogWrite("Enemy Attack:%s added to registry with name:%s, and ID:%i",
                stringID.c_str(),
                enemyAttack.attackName.unicode(),
                enemyAttack.attackID,
            );*/
            srLogWrite("setting attack %s to be enemy action %d", stringID.c_str(), attackID);
            setCommandAction(assembleGDataKey(CMD_ENEMY_ACTION), stringID, attackID);
        }
        // For this to work we need to actually be able to set a specific index, not just append
        //In the event the string ID already exists (kernel action) just ref it
    }
    srLogWrite("scene fully registered!");
}

void calculateEnemyStats(SrEnemyData& enemy) {
    const auto& gameEnemy = enemy.enemyData;
    auto& stats = enemy.enemyStats;
    SrActorStat stat = { 1, 1 };
    for (const auto& element : gContext.stats.named_registry) {
        stats[element.first] = stat;
    }
    stats[StatNames::HP].baseValue = gameEnemy.maxHP;
    stats[StatNames::MP].baseValue = gameEnemy.maxMP;
    stats[StatNames::STRENGTH].baseValue = gameEnemy.strength;
    stats[StatNames::VITALITY].baseValue = gameEnemy.defense;
    stats[StatNames::MAGIC].baseValue = gameEnemy.magic;
    stats[StatNames::SPIRIT].baseValue = gameEnemy.mDefense;
    stats[StatNames::DEXTERITY].baseValue = gameEnemy.dexterity;
    stats[StatNames::LUCK].baseValue = gameEnemy.luck;
    stats[StatNames::EVADE].baseValue = gameEnemy.enemyEvade;
    stats[StatNames::MEVADE].baseValue = gameEnemy.enemyEvade;

    auto elements = &(gameEnemy.elementTypes[0]);
    auto elementModifiers = &(gameEnemy.elementModifiers[0]);
    for (auto idx = 0; idx < 8; idx++) {
        auto element = elements[idx];
        auto resistance = getResistanceFromElementalModifier(elementModifiers[idx]);
        switch (element) {
        case ELM_FIRE: {
            stats[StatNames::FIRE_RES].baseValue = resistance;
            break;
        }
        case ELM_ICE: {
            stats[StatNames::ICE_RES].baseValue = resistance;
            break;
        }
        case ELM_THUNDER: {
            stats[StatNames::LIGHT_RES].baseValue = resistance;
            break;
        }
        case ELM_EARTH: {
            stats[StatNames::EARTH_RES].baseValue = resistance;
            break;
        }
        case ELM_WIND: {
            stats[StatNames::WIND_RES].baseValue = resistance;
            break;
        }
        case ELM_WATER: {
            stats[StatNames::WATER_RES].baseValue = resistance;
            break;
        }
        case ELM_POISON: {
            stats[StatNames::POISON_RES].baseValue = resistance;
            break;
        }
        case ELM_GRAVITY: {
            stats[StatNames::GRAVITY_RES].baseValue = resistance;
            break;
        }
        }
    }
    calculateFinalStats(enemy);
}


void calculateFinalStats(SrEnemyData& enemy) {
    for (const auto& element : gContext.stats.named_registry) {
        enemy.enemyStats[element.first].statValue = enemy.enemyStats[element.first].baseValue;
    }
}

i32 getResistanceFromElementalModifier(u8 modifier) {
    switch (modifier) {
    case 0: {
        return -255; // not a reachable value, but signals instant death
    }
    case 2: {
        return -50;
    }
    case 4: {
        return 50;
    }
    case 5: {
        return 100;
    }
    case 6: {
        return 150;
    }
    case 7: {
        return 255; // not a reachable value, but signals full cure behavior
    }
    default: {
        return 0;
    }
    }
}
