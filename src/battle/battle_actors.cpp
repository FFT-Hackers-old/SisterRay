#include "battle_actors.h"
#include "scene_globals.h"
#include "../impl.h"

#define ACTOR_COUNT 10
#define ENEMY_ACTOR_COUNT 6
#define CHARACTER_COUNT 10

SrBattleActors::SrBattleActors() {
    for (u8 actorIdx = 0; actorIdx < CHARACTER_COUNT; actorIdx++) {
        partyActors[actorIdx] = SrPartyBattleActor();
    }
    for (u8 enemyActorIdx = 0; enemyActorIdx < ENEMY_ACTOR_COUNT; enemyActorIdx++) {
        enemyActors[enemyActorIdx] = SrBattleActor();
    }
}

void SrBattleActors::setActivePartyActor(u8 partyIdx, u8 characterID) {
    if (partyIdx < 3)
        activeParty[partyIdx] = characterID;
}

void SrBattleActors::activatePartyActor(u8 partyIdx) {
    auto gameActor = getActiveBattleActor(partyIdx);
    auto srActor = getSrBattleActor(partyIdx);

    *gameActor.actorBattleVars = *srActor.actorBattleVars;
    *gameActor.actorTimers = *srActor.actorTimers;
    *gameActor.party10 = *srActor.party10;
    *gameActor.party34 = *srActor.party34;
    *gameActor.weaponCtx = *srActor.weaponCtx;
}

void SrBattleActors::activateEnemyActor(u8 enemyIdx) {
    auto& gameActor = getActiveBattleActor(enemyIdx + 4);
    auto& srActor = getSrBattleActor(enemyIdx + 4);

    *gameActor.actorBattleVars = *srActor.actorBattleVars;
    *gameActor.actorTimers = *srActor.actorTimers;
}

void SrBattleActors::savePartyActor(u8 partyIdx) {
    if (partyIdx > 3)
        return;

    auto gameActor = getActiveBattleActor(partyIdx);
    auto srActor = getSrBattleActor(partyIdx);

    *srActor.actorBattleVars = *gameActor.actorBattleVars;
    *srActor.actorTimers = *gameActor.actorTimers;
    *srActor.party10 = *gameActor.party10;
    *srActor.party34 = *gameActor.party34;
    *srActor.weaponCtx = *gameActor.weaponCtx;
}

void SrBattleActors::saveEnemyActor(u8 enemyIdx) {
    auto& gameActor = getActiveBattleActor(enemyIdx + 4);
    auto& srActor = getSrBattleActor(enemyIdx + 4);

    *srActor.actorBattleVars = *gameActor.actorBattleVars;
    *srActor.actorTimers = *gameActor.actorTimers;
}

void SrBattleActors::initializePartyActor(u8 partyIdx, u8 characterID) {
    u16* word_9A889E = (u16*)0x9A889E;
    auto partyActor = getSrBattleActor(partyIdx);
    const auto& srPartyMember = gContext.party.getSrPartyMember(partyIdx);
    auto characterRecord = getCharacterRecordWithID(characterID);
    if (characterRecord == nullptr) {
        continue;
    }

    setActivePartyActor(partyIdx, characterID);

    auto actorTimers = partyActor.actorTimers;
    auto actorBattleVars = partyActor.actorBattleVars;
    auto party34 = partyActor.party34;
    auto party10 = partyActor.party10;
    const auto& partyMember = srPartyMember.gamePartyMember;
    auto partyWeaponCtx = partyActor.weaponCtx;

    party10->characterID = -1;
    actorTimers->unkActorFlags = 1;
    actorTimers->field_C = -1;
    actorTimers->field_F = -1;
    party10->characterID = characterID;
    actorBattleVars->index = characterID;
    actorBattleVars->characterID = characterID + 0x10;
    actorBattleVars->level = characterRecord->level;
    actorBattleVars->idleAnimHolder = 0;
    actorBattleVars->apValue = 8;
    actorBattleVars->damageAnimID = 5;
    actorBattleVars->stateFlags = 8;
    party34->characterRecord = characterRecord;
    if (!(characterRecord->character_row & 1))
        actorBattleVars->stateFlags |= 0x40u;
    setPartyStats(partyIdx, partyActor);
    actorBattleVars->backDamageMult = 16;
    actorBattleVars->statusMask = characterRecord->outOfBattleStatus & 0x30;
    actorBattleVars->initalStatusMasks = 0;
    actorTimers->statusImmuneMask = partyMember.immuneStatusesMask;
    setWeaponData(partyIdx, partyActor);
    // handleAccessorySpecialEffect(partyActorID, characterRecordPtr->equipped_accessory);
    actorBattleVars->gilStolen = 0;
    actorBattleVars->itemStolen = -1;
    actorTimers->unkActorFlags &= 0xFDu;

    if (partyMember->actorFlags & 4)
        partyWeaponCtx->targetFlags &= 0xDFu;
    if (!(partyWeaponCtx->targetFlags & 0x20))
        actorTimers->unkActorFlags |= 2u;

    actorTimers->charATBValue = 0;

    party34->limitLevelIdx = 0xFF;
    if (characterID < 9) {
        u8 limitLevel = characterRecord->activeLimitLevel;
        if (limitLevel >= 1 && limitLevel <= 4) {
            party34->limitBar = characterRecord->limit_bar_progress;
            party34->limitBarCopy = party34->limitBar;
            party34->limitLevelIdx = limitLevelIdx;
            const auto& charGrowth = gContext.characters.getResource(characterID).characterGrowth;
            switch (limitLevel) {
            case 1:
                party34->limitLevelDivisor = charGrowth.level1Divisor;
                break;
            case 2:
                party34->limitLevelDivisor = charGrowth.level2Divisor;
                break;
            case 3:
                party34->limitLevelDivisor = charGrowth.level3Divisor;
                break;
            case 4:
                party34->limitLevelDivisor = charGrowth.level4Divisor;
                break;
            }
            partyMember.limitGuage = party34.limitBreakBar << 8;
            partyMember.activeLimitLevel = characterRecord->activeLimitLevel;
            // setLearnedLimits(characterID, characterRecord->learnedLimits, partyMember->enabledLimitBytes);
        }
    }

    actorBattleVars->actorFlags |= 8u;
    if (!actorBattleVars->currentHP) {
        actorBattleVars->statusMasks |= 1;
    }

    //sub_437370(partyIdx); Routine sets up attack data for switching to limit. Won't be needed
    if (party34->limitBar == 255) {
        if (party34->limitIdx != 255) {
            *word_9A889E |= 1 << partyIdx;
            actorTimers->field_8 |= 1;
            actorTimers->enabledCommandsMask |= 1u;
        }
        actorTimers->field_8 &= 0xFFFEu;
    }

    if (actorBattleVars->statusMask)
        sub_5C7DB0(partyIdx); //dispatches an animation event
    
    // These are used when learning a new ESkill, this should be handled differently
    //party34->learnedEnemySkills = getLearnedEnemySkills(characterRecord);
    //party34->knownEnemySkills = party34->learnedEnemySkills;
    gContext.party.initPartyBattleFields(partyIdx, partyActor);
    activatePartyActor(partyIdx);
}

void SrBattleActors::initializeEnemyActor(u8 enemyIdx) {
    u8* byte_9AAD2C = (u8*)0x9AAD2C;
    auto enemyActor = getSrBattleActor(enemyIdx + 4);
    auto actorTimers = enemyActor.actorTimers;
    auto actorBattleVars = enemyActor.actorBattleVars;
    auto& actorFormation = *getInBattleFormationActorData(enemyIdx);
    auto enemyModelID = actorFormation.enemyID;
    actorBattleVars->index = -1;
    actorBattleVars->formationID = -1;
    actorBattleVars->stateFlags = 0;
    actorBattleVars->statusMask = 0;
    actorBattleVars->cameraData = -1;

    byte_9A9AAC[8 * enemyActorIdx] = -1;
    for (u8 l = 0; l < 16; ++l)
        *(&actorTimers->field_10 + l) = 0;
    for (u8 m = 0; m < 8; ++m)
        *(&actorTimers->field_20 + m) = 0;

    if (enemyModelID != 0xFFFF) {
        actorBattleVars->formationID = enemyModelID;
        auto sceneRelativeID = 0;
        for (auto idx = 0; idx < 3; ++idx) {
            if (getInBattleFormationEnemyModels()->EnemyIds[idx] == enemyModelID) {
                sceneRelativeID = idx;
                break;
            }
        }
        byte_9AAD2C[sceneRelativeID]++;
        auto sceneEnemyDataPtr = getInBattleActorEnemyData(sceneRelativeID);
        actorFormation.enemyID = sceneRelativeID;
        actorBattleVars->index = sceneRelativeID;
        actorBattleVars->maxHP = sceneEnemyDataPtr->enemyHP;
        actorBattleVars->currentHP = actorBattleVars->maxHP;
        actorBattleVars->maxMP = sceneEnemyDataPtr->enemyMP;
        actorBattleVars->currentMP = actorBattleVars->maxMP;
        actorBattleVars->physAtk = sceneEnemyDataPtr->enemyStrength;
        actorBattleVars->magAtk = sceneEnemyDataPtr->enemyMagic;
        actorBattleVars->defense = 2 * sceneEnemyDataPtr->enemyDefense;
        actorBattleVars->mdefense = 2 * sceneEnemyDataPtr->enemyMDefense;
        actorBattleVars->pEvade = sceneEnemyDataPtr->enemyEvade;
        actorBattleVars->dexterity = sceneEnemyDataPtr->enemyDex;
        actorBattleVars->luck = sceneEnemyDataPtr->enemyLuck;
        actorBattleVars->level = sceneEnemyDataPtr->enemyLevel;
        actorBattleVars->backDamageMultiplier = sceneEnemyDataPtr->backDamageMultiplier;
        actorBattleVars->GilValue = sceneEnemyDataPtr->gilAward;
        actorBattleVars->ExpValue = sceneEnemyDataPtr->expAward;
        actorBattleVars->damagedAnim = 1;
        actorBattleVars->unknown6 = 1;
        actorBattleVars->apValue = 2;
        actorBattleVars->idleAnim = 0;
        actorBattleVars->statusMasks = 0;
        actorBattleVars->initStatusMasks = 0;
        actorBattleVars->gilStolen = 0;
        actorBattleVars->itemStolen = -1;
        actorBattleVars->actorFlags = formationActorFlags[4 * enemyActorIdx] & 0x1F;
        actorBattleVars->actorRow = formationActorRow[16 * enemyActorIdx];
        actorTimers->battleEnemyData = sceneEnemyDataPtr;
        actorTimers->field_D = -1;
        actorTimers->field_C = -1;
        actorTimers->field_F = -1;
        actorTimers->statusImmuneMask = ~sceneEnemyDataPtr->statusImmunityMask;

        // Initialize the formation relative ID for enemies
        actorBattleVars->charID = 0;
        for (auto idx = 0; idx < enemyIdx; ++idx) {
            if (enemyActors[idx + 4].battleActor.gameAIState.formationID == actorBattleVars->formationID)
                ++actorBattleVars->characterID;
        }
        /* for (manipulatedAttackIDs = 0; manipulatedAttackIDs < 3; ++manipulatedAttackIDs) {
            manipulateCommanDataPtr = &manipulateCommandData[96 * enemyActorIdx + 6 * manipulatedAttackIDs];
            manipulatedAttackID = sceneEnemyDataPtr->manipAttackIDs[manipulatedAttackIDs];
            manipulateCommandData[96 * enemyActorIdx + 6 * manipulatedAttackIDs] = -1;
            manipulateCommanDataPtr[1] = 0;
            manipulateCommanDataPtr[2] = 0;
            manipulateCommanDataPtr[3] = 3;
            if (manipulatedAttackID != 0xFFFF)
            {
                for (sceneAttackIdx = 0; sceneAttackIdx < 32; ++sceneAttackIdx)
                {
                    if ((unsigned __int16)sceneActionIDs[sceneAttackIdx] == manipulatedAttackID)
                    {
                        v1 = byte_9A90D0[28 * sceneAttackIdx];
                        if (byte_9A90D0[28 * sceneAttackIdx])
                            v1 ^= 2u;
                        *manipulateCommanDataPtr = sceneAttackIdx;
                        manipulateCommanDataPtr[2] = v1;
                        manipulateCommanDataPtr[3] = 0;
                        break;
                    }
                }
            }
        }
        for (commandSlot = 3; commandSlot < 16; ++commandSlot)// pad out the rest of an actors commands
        {
            manipulateCommandData[96 * enemyActorIdx + 6 * commandSlot] = -1;
            v0 = &manipulateCommandData[96 * enemyActorIdx + 6 * commandSlot];
            v0[1] = 0;
            v0[2] = 0;
            v0[3] = 3;
        }*/
        actorTimers->unkActorFlags = 0;
        if (byte_9AAD2C[actorFormation.enemyID] > 1)
            actorTimers->field_F = actorBattleVars->characterID;

        activateEnemyActor(enemyIdx);
    }
}

void SrBattleActors::swapPartyActors(u8 partyIdx, u8 newCharacterID) {
    savePartyActor(partyIdx);
    setActivePartyActor(partyIdx, newCharacterID);
    activatePartyActor(partyIdx);
}

ActorBattleState SrBattleActors::getSrBattleActor(u8 actorIdx) {
    ActorBattleState actorState = ActorBattleState();
    if (actorIdx < 3) {
        auto& partyActor = partyActors[activeParty[actorIdx]];
        actorState.actorBattleVars = &(partyActor.battleActor.gameAIState);
        actorState.actorTimers = &(partyActor.battleActor.gameTimerState);
        actorState.battleStats = &(partyActor.battleActor.battleStats);
        actorState.party10 = &(partyActor.party10);
        actorState.party34 = &(partyActor.party34);
        actorState.weaponCtx = &(partyActor.weaponCtx);
        return actorState;
    }
    actorState.actorBattleVars = enemyActors[actorIdx - 4].battleActor.gameAIState;
    actorState.actorTimers = enemyActors[actorIdx - 4].battleActor.gameTimerState;
    actorState.battleStats = &(enemyActors[actorIdx - 4].battleActor.battleStats);
    actorState.party10 = nullptr;
    actorState.party34 = nullptr;
    actorState.weaponCtx = nullptr;
    return actorState;
}

ActorBattleState SrBattleActors::getActiveBattleActor(u8 actorIdx) {
    ActorBattleState actorState = ActorBattleState();
    actorState.actorBattleVars = getActorBattleVars(actorIdx);
    actorState.actorTimers = getActorTimerBlock(actorIdx);
    if (actorIdx < 3) {
        actorState.battleStats = &(enemyActors[actorIdx - 4].battleActor.battleStats);
        actorState.party10 = getBattleParty10(actorIdx);
        actorState.party34 = getBattleParty34(actorIdx);
        actorState.weaponCtx = getBattleWeaponCtx(actorIdx);
        return actorState;
    }
    actorState.battleStats = &(enemyActors[actorIdx].battleActor.battleStats);
    actorState.party10 = nullptr;
    actorState.party34 = nullptr;
    actorState.weaponCtx = nullptr;
    return actorState;
}


u8* SrBattleActors::getActorRandomVarBuffer(u8 actorIdx) {
    return &(actorScriptVariables[actorIdx].buffer[0]);
}

ActorTimerData* getActorTimerBlock(u8 actorIdx) {
    return &(G_ACTOR_TIMER_ARRAY[actorIdx]);
}


ActorBattleVars* getActorBattleVars(u8 actorIdx) {
    return &(AI_BATTLE_CONTEXT->actorAIStates[actorIdx]);
}

BattleParty34* getBattleParty34(u8 actorIdx) {
    return &(G_BATTLE_PARTY34_ARRAY[actorIdx]);
}

BattleParty10* getBattleParty10(u8 actorIdx) {
    return &(G_BATTLE_PARTY10_ARRAY[actorIdx]);
}

BattleWeaponCtx* getBattleWeaponCtx(u8 actorIdx) {
    return &(G_BATTLE_PARTY10_ARRAY[actorIdx]);
}

const SrBattleStat& getSrBattleStat(u8 actorID, std::string statName) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    return statMap[statName];
}

SrBattleStat& getMutableSrBattleStat(u8 actorID, std::string statName) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    return statMap[statName];
}

void setActorBattleStat(u8 actorID, std::string statName, u32 value) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    auto& stat = statMap[statName];
    stat.statValue = (value <= stat.maxValue) ? value : stat.maxValue;
}

CharacterRecord* getCharacterRecordWithID(u8 characterID) {
    for (u8 characterRecordIdx = 0; characterRecordIdx < 9; ++characterRecordIdx) {
        auto characterRecord = &(CHARACTER_RECORD_ARRAY[characterRecordIdx]);
        if (battleFlags & 0x40)
            qmemcpy(&byte_9ACAD4, &charDataArray[characterRecordIdx], 0x84u);

        if (characterRecord->characterID == characterID)
            return characterRecord;
    }
    return nullptr;
}

#define DEX_NORMALIZATION 75
void initializePlayerActors() {
    u16* G_DEX_NORMALIZATION = (u16*)0x9AAD00;
    auto partyDex = 0;
    auto partyCount = 0;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.actorPartyMask = 0;
    for (u8 partyIdx = 0; partyIdx < 3; partyIdx++) {
        u8 characterID = G_SAVEMAP_PARTY[partyIdx];
        aiBattleContext.actorPartyMask |= 1 << partyIdx;
        gContext.battleActors.initializePartyActor(partyIdx, characterID);
    }
    if (partyCount)
        *G_DEX_NORMALIZATION = DEX_NORMALIZATION;
}

void initializeEnemyActors() {
    u8* byte_9AAD2C = (u8*)0x9AAD2C;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.unkMask = 0;
    for (auto sceneRelativeIdx = 0; sceneRelativeIdx < 3; ++sceneRelativeIdx)
        byte_9AAD2C[sceneRelativeIdx] = 0;

    for (auto enemyIdx = 0; enemyIdx < 6; ++enemyIdx) {
        aiBattleContext.unkMask |= 1 << (enemyIdx + 4);
        gContext.battleActors.initializeEnemyActor(enemyIdx);
    }
}

void setPartyStats(u8 partyIdx, ActorBattleState& partyActor) {
    const auto& srPartyMember = gContext.party.getSrPartyMember(partyIdx);
    auto& actorBattleVars = partyActor.actorBattleVars;
    auto& stats = srPartyMember.srPartyMember->playerStats;
    actorBattleVars->currentHP = srPartyMember.gamePartyMember->currentHP;
    actorBattleVars->currentMP = srPartyMember.gamePartyMember->currentMP;
    partyActor.party34->currentHP = actorBattleVars->currentHP;
    partyActor.party34->currentMP = actorBattleVars->currentMP;
    actorBattleVars->dexterity = stats["DEX"].statValue;
    actorBattleVars->luck = stats["LCK"].statValue;
    actorBattleVars->maxHP = stats["HP"].statValue;
    actorBattleVars->maxMP = stats["MP"].statValue;
    actorBattleVars->physAtk = stats["STR"].statValue;
    actorBattleVars->magAtk = stats["MAG"].statValue;
    actorBattleVars->defense = stats["VIT"].statValue;
    actorBattleVars->mdefense = stats["SPR"].statValue;
    if (!actorBattleVars->physAtk)
        actorBattleVars->physAtk = 1;

    partyActor.party34->maxHP = actorBattleVars->maxHP;
    partyActor.party34->maxMP = actorBattleVars->maxMP;

    if (srPartyMember.gamePartyMember->actorFlags & 8) {
        partyActor.party34->field_16 = 999;
        partyActor.party34->field_14 = 9999;
    }
    else {
        partyActor.party34->field_16 = 9999;
        partyActor.party34->field_14 = 999;
    }
    actorBattleVars->pEvade = stats["PEV"].statValue;
    actorBattleVars->mEvade = stats["MEV"].statValue;
}

void setWeaponData(u8 partyIdx, ActorBattleState& partyActor) {
    const auto& srPartyMember = gContext.party.getSrPartyMember(partyIdx);
    auto& partyMember = srPartyMember.gamePartyMember;
    auto& partyWeaponCtx = *partyActor.weaponCtx;

    partyWeaponCtx.targetFlags = partyMember.weaponData.targetFlags;
    partyWeaponCtx.attackEffectID = partyMember.weaponData.attackEffectID;
    partyWeaponCtx.damageCalculation = partyMember.weaponData.damageCalculation;
    partyWeaponCtx.weaponHitRate = partyMember.weaponData.weaponHitRate;
    partyWeaponCtx.impactEffectID = partyMember.weaponData.impactEffectID;
    partyWeaponCtx.criticalRate = partyMember.weaponData.criticalRate;
    partyWeaponCtx.attackElementMask = partyMember.attackElementsMask | partyMember.weaponData.attackElementMask;
    partyWeaponCtx.cameraMovement = partyMember.weaponData.cameraMovement;
    partyWeaponCtx.specialAttackFlags = partyMember.weaponData.specialAttackFlags;
    partyWeaponCtx.attackStatusMask = partyMember.attackStatusesMask;

    u16 soundId = partyMember.weaponData.normalSoundID;
    u8 accessMaskCheck = 1;

    auto soundID = partyMember.weaponData.normalSoundID;
    if (accessMaskCheck & partyMember.weaponData.soundAccessMask)
        soundID |= 0x100u;
    partyWeaponCtx.normalSoundID = soundID;
    accessMaskCheck *= 2;

    soundID = partyMember.weaponData.critSoundID;
    if (accessMaskCheck & partyMember.weaponData.soundAccessMask)
        soundID |= 0x100u;
    partyWeaponCtx.critSoundID = soundID;
    accessMaskCheck *= 2;

    soundID = partyMember.weaponData.missSoundID;
    if (accessMaskCheck & partyMember.weaponData.soundAccessMask)
        soundID |= 0x100u;
    partyWeaponCtx.missSoundID = soundID;
    accessMaskCheck *= 2;
}
