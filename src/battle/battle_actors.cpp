#include "battle_actors.h"
#include "scene_globals.h"
#include "battle_engine_interface.h"
#include "animation_script/animation_script_interface.h"
#include "battle_models/battle_models_api.h"
#include "../impl.h"
#include "../gamedata/stat_names.h"
#include "../gamedata/status_names.h"
#include "../gamedata/damage_callback_utils.h"
#include "../gamedata/summons.h"
#include "../battle/time/battle_time.h"
#include "../events/event_bus_interface.h"

#define ACTOR_COUNT 10
#define ENEMY_ACTOR_COUNT 6
#define CHARACTER_COUNT 10

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

typedef void(*SRPFN_SUB5C7DB0)(u8);
#define sub_5C7DB0  (SRPFN_SUB5C7DB0(0x5C7DB0))
void SrBattleActors::initializePartyActor(u8 partyIdx, u8 characterID) {
    u16* word_9A889E = (u16*)0x9A889E;
    setActivePartyActor(partyIdx, characterID);
    initializePartyCharacter(characterID);
    auto& partyActor = getSrBattleActor(partyIdx);
    gContext.party.initPartyBattleFields(partyIdx, partyActor);

    if (partyActor.actorBattleVars->statusMask)
        sub_5C7DB0(partyIdx); //dispatches an animation event
    // These are used when learning a new ESkill, this should be handled differently
    //party34->learnedEnemySkills = getLearnedEnemySkills(characterRecord);
    //party34->knownEnemySkills = party34->learnedEnemySkills;

    InitBattleActorEvent initActorEvent{partyIdx, characterID, &getSrBattleCharacterActor(characterID) };
    gContext.eventBus.dispatch(INIT_PLAYER_BATTLE_ACTOR, &initActorEvent);
    activatePartyActor(partyIdx);
}

bool SrBattleActors::isActorInBattle(u8 actorIdx) {
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    return aiBattleContext.allActorsMask & (1 << actorIdx);
}

void SrBattleActors::initializePartyCharacter(u8 characterID) {
    auto& partyActor = partyActors[characterID];
    const auto& srPartyMember = gContext.party.getSrCharacter(characterID);
    auto characterRecord = getCharacterRecordWithID(characterID);
    if (characterRecord == nullptr) {
        return;
    }

    auto& actorTimers = partyActor.battleActor.gameTimerState;
    auto& actorBattleVars = partyActor.battleActor.gameAIState;
    auto& party34 = partyActor.party34;
    auto& party10 = partyActor.party10;
    const auto& partyMember = srPartyMember.gamePartyMember;
    auto partyWeaponCtx = partyActor.weaponCtx;

    party10.characterID = -1;
    actorTimers.unkActorFlags = 1;
    actorTimers.field_C = -1;
    actorTimers.field_F = -1;
    party10.characterID = characterID;
    actorBattleVars.index = characterID;
    actorBattleVars.characterID = characterID + 0x10;
    actorBattleVars.level = characterRecord->level;
    actorBattleVars.idleAnimHolder = 0;
    actorBattleVars.apValue = 8;
    actorBattleVars.damageAnimID = 5;
    actorBattleVars.stateFlags = 8;
    party34.characterRecord = characterRecord;
    if (!(characterRecord->character_row & 1)) {
        actorBattleVars.stateFlags |= 0x40u;
        actorBattleVars.actorRow = characterRecord->character_row;
    }
    setPartyStats(characterID, partyActor);
    actorBattleVars.backDamageMult = 16;
    actorBattleVars.statusMask = characterRecord->outOfBattleStatus & 0x30;
    actorBattleVars.initalStatusMasks = 0;
    actorTimers.innateStatusMask = partyMember->immuneStatusesMask;
    setWeaponData(characterID, getSrBattleCharacterActor(characterID));
    // handleAccessorySpecialEffect(partyActorID, characterRecordPtr->equipped_accessory);
    actorBattleVars.gilStolen = 0;
    actorBattleVars.itemStolen = -1;
    actorTimers.unkActorFlags &= 0xFDu;

    if (partyMember->actorFlags & 4)
        partyWeaponCtx.targetFlags &= 0xDFu;
    if (!(partyWeaponCtx.targetFlags & 0x20))
        actorTimers.unkActorFlags |= 2u;

    actorTimers.charATBValue = 0;

    party34.limitLevelIdx = 0xFF;
    if (characterID < 9) {
        u8 limitLevel = characterRecord->activeLimitLevel;
        if (limitLevel >= 1 && limitLevel <= 4) {
            party34.limitBar = characterRecord->limit_bar_progress;
            party34.limitBarCopy = party34.limitBar;
            party34.limitLevelIdx = limitLevel - 1;
            const auto& charGrowth = gContext.characters.getResource(characterID).characterGrowth;
            switch (limitLevel) {
            case 1:
                party34.limitLevelDivisor = charGrowth.level1Divisor;
                break;
            case 2:
                party34.limitLevelDivisor = charGrowth.level2Divisor;
                break;
            case 3:
                party34.limitLevelDivisor = charGrowth.level3Divisor;
                break;
            case 4:
                party34.limitLevelDivisor = charGrowth.level4Divisor;
                break;
            }
            // setLearnedLimits(characterID, characterRecord->learnedLimits, partyMember->enabledLimitBytes);
        }
    }

    actorBattleVars.stateFlags |= 8u;
    if (!actorBattleVars.currentHP) {
        actorBattleVars.statusMask |= 1;
    }

    //sub_437370(partyIdx); Routine sets up attack data for switching to limit. Won't be needed
    u16* word_9A889E = (u16*)0x9A889E;
    if (party34.limitBar == 255) {
        if (party34.limitLevelIdx != 255) {
            *word_9A889E |= 1 << characterID;
            actorTimers.limitFastATBMask |= 1;
            actorTimers.activeCommandsMask |= 1u;
        }
        actorTimers.limitFastATBMask &= 0xFFFEu;
    }

    gContext.party.initCharacterBattleFields(characterID, getSrBattleCharacterActor(characterID));
    srSetActorTimers(getSrBattleCharacterActor(characterID));

    actorTimers.currentHP = actorBattleVars.currentHP;
    actorTimers.currentMP = actorBattleVars.currentMP;
    partyActor.battleActor.activeStatuses.clear();

    InitBattleActorEvent timerEvent{ 0xFF, characterID, &getSrBattleCharacterActor(characterID),  true };
    dispatchEvent(INIT_BATTLE_ACTORS, &timerEvent);
    // These are used when learning a new ESkill, this should be handled differently
    //party34->learnedEnemySkills = getLearnedEnemySkills(characterRecord);
    //party34->knownEnemySkills = party34->learnedEnemySkills;
}


void SrBattleActors::initializePartySummon(u8 summonIdx) {
    auto& summonActor = summonActors[summonIdx];
    auto summonCtx = gContext.party.getSummonCtx(summonIdx);
    if (!summonCtx.summonActive)
        return;

    const auto& summonPartyMember = gContext.party.getSrSummon(summonIdx);

    auto& actorTimers = summonActor.battleActor.gameTimerState;
    auto& actorBattleVars = summonActor.battleActor.gameAIState;
    auto& party34 = summonActor.party34;
    auto& party10 = summonActor.party10;
    const auto& partyMember = summonPartyMember.gamePartyMember;
    auto partyWeaponCtx = summonActor.weaponCtx;

    party10.characterID = -1;
    actorTimers.unkActorFlags = 1;
    actorTimers.field_C = -1;
    actorTimers.field_F = -1;
    party10.characterID = summonIdx;
    actorBattleVars.index =summonIdx;
    actorBattleVars.characterID = summonIdx + 0x10;
    actorBattleVars.level = min(10 + (gContext.party.getSummonCtx(summonIdx).cumulativeAP / 1000), 99);
    actorBattleVars.idleAnimHolder = 0;
    actorBattleVars.apValue = 8;
    actorBattleVars.damageAnimID = 5;
    actorBattleVars.stateFlags = 8;
    party34.characterRecord = nullptr;
    setBattleStats(summonPartyMember, summonActor);
    actorBattleVars.backDamageMult = 16;
    actorBattleVars.statusMask = 0;
    actorBattleVars.initalStatusMasks = 0;
    actorTimers.innateStatusMask = partyMember->immuneStatusesMask;
    // setWeaponData(characterID, getSrBattleCharacterActor(characterID));
    // handleAccessorySpecialEffect(partyActorID, characterRecordPtr->equipped_accessory);
    actorBattleVars.gilStolen = 0;
    actorBattleVars.itemStolen = -1;
    actorTimers.unkActorFlags &= 0xFDu;

    if (partyMember->actorFlags & 4)
        partyWeaponCtx.targetFlags &= 0xDFu;
    if (!(partyWeaponCtx.targetFlags & 0x20))
        actorTimers.unkActorFlags |= 2u;

    actorTimers.charATBValue = 0;

    party34.limitLevelIdx = 0xFF;
    u8 limitLevel = 1;
    if (limitLevel >= 1 && limitLevel <= 4) {
        party34.limitBar = 0;
        party34.limitBarCopy = party34.limitBar;
        party34.limitLevelIdx = limitLevel - 1;
        party34.limitLevelDivisor = gContext.party.getSummonCtx(summonIdx).maxLevel;
        // setLearnedLimits(characterID, characterRecord->learnedLimits, partyMember->enabledLimitBytes);
    }

    actorBattleVars.stateFlags |= 8u;
    if (!actorBattleVars.currentHP) {
        actorBattleVars.statusMask |= 1;
    }

    u16* word_9A889E = (u16*)0x9A889E;

    gContext.party.initSummonBattleFields(summonIdx, getSrBattleSummonActor(summonIdx));
    srSetActorTimers(getSrBattleSummonActor(summonIdx));
    actorTimers.currentHP = actorBattleVars.currentHP;
    actorTimers.currentMP = actorBattleVars.currentMP;
    summonActor.battleActor.activeStatuses.clear();
}


void SrBattleActors::initializeEnemyActor(u8 enemyIdx) {
    srLogWrite("--------------------START INIT ENEMY---------------------------");
    u8* byte_9AAD2C = (u8*)0x9AAD2C;
    u8* byte_9A9AAC = (u8*)0x9A9AAC;
    auto enemyActor = getSrBattleActor(enemyIdx + 4);
    auto actorTimers = enemyActor.actorTimers;
    auto actorBattleVars = enemyActor.actorBattleVars;
    auto& actorFormation = *getInBattleFormationActorData(enemyIdx);
    auto enemyModelID = actorFormation.enemyID;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    actorBattleVars->index = -1;
    actorBattleVars->formationID = -1;
    actorBattleVars->stateFlags = 0;
    actorBattleVars->statusMask = 0;
    actorBattleVars->cameraData = -1;

    byte_9A9AAC[8 * enemyIdx] = -1;
    for (u8 l = 0; l < 16; ++l)
        actorTimers->eventTimers[l] = 0;
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

        aiBattleContext.enemyActorsMask |= 1 << (enemyIdx + 4);
        byte_9AAD2C[sceneRelativeID]++;
        auto sceneEnemyDataPtr = getInBattleActorEnemyData(sceneRelativeID);
        actorFormation.enemyID = sceneRelativeID;
        actorBattleVars->index = sceneRelativeID;
        const auto& formationEnemy = *getInBattleFormationActorData(enemyIdx);
        const auto& srEnemy = gContext.enemies.getResource(getUniqueEnemyID(formationEnemy.enemyID));
        enemyActors[enemyIdx].enemyData = srEnemy;
        setEnemyStats(enemyIdx, enemyActor);
        actorBattleVars->level = sceneEnemyDataPtr->enemyLevel;
        actorBattleVars->backDamageMult = sceneEnemyDataPtr->backDamageMultipler;
        actorBattleVars->gilValue = sceneEnemyDataPtr->gilAward;
        actorBattleVars->expValue = sceneEnemyDataPtr->expAward;
        actorBattleVars->damageAnimID = 1;
        actorBattleVars->unknown6 = 1;
        actorBattleVars->apValue = 2;
        actorBattleVars->idleAnimScript = 0;
        actorBattleVars->statusMask = 0;
        actorBattleVars->initalStatusMasks = 0;
        actorBattleVars->gilStolen = 0;
        actorBattleVars->itemStolen = -1;
        actorBattleVars->stateFlags = actorFormation.initialStateFlags & 0x1F;
        actorBattleVars->actorRow = actorFormation.actorRow;
        actorTimers->battleEnemyData = sceneEnemyDataPtr;
        actorTimers->field_D = -1;
        actorTimers->field_C = -1;
        actorTimers->field_F = -1;
        actorTimers->innateStatusMask = ~sceneEnemyDataPtr->statusImmunityMask;

        // Initialize the formation relative ID for enemies
        actorBattleVars->characterID = 0;
        for (auto idx = 0; idx < enemyIdx; ++idx) {
            if (enemyActors[idx].battleActor.gameAIState.formationID == actorBattleVars->formationID)
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
        for (u8 enemyIdx = 0; enemyIdx < 6;  enemyIdx++) {
            if (byte_9AAD2C[getInBattleFormationActorData(enemyIdx)->enemyID] > 1)
                enemyActors[enemyIdx].battleActor.gameTimerState.field_F = enemyActors[enemyIdx].battleActor.gameAIState.characterID;
        }

        srSetActorTimers(enemyActor);

        actorTimers->currentHP = actorBattleVars->currentHP;
        actorTimers->currentMP = actorBattleVars->currentMP;

        auto& statuses = enemyActors[enemyIdx].battleActor.activeStatuses;
        statuses.clear();
        srLogWrite("ENEMY STATUSES AFTER CLEAR");
        for (auto& targetStatus : enemyActors[enemyIdx].battleActor.activeStatuses) {
            srLogWrite("ENEMY STATUSES AFTER INIT: %s", targetStatus.statusName.c_str());
        }
        activateEnemyActor(enemyIdx);
        InitBattleActorEvent timerEvent{ enemyIdx, 0xFF, &enemyActor, true };
        dispatchEvent(INIT_BATTLE_ACTORS, &timerEvent);
        srLogWrite("--------------------END INIT ENEMY---------------------------");
    }
}

void SrBattleActors::swapPartyActors(u8 partyIdx, u8 newCharacterID) {
    savePartyActor(partyIdx);
    setActivePartyActor(partyIdx, newCharacterID);
    activatePartyActor(partyIdx);
}

void SrBattleActors::swapPartyToSummon(u8 summonIdx){
    savePartyActors();
    actorIsSummon[1] = true;
    activeSummonIdx = summonIdx;
    auto gameActor = getActiveBattleActor(1);
    auto srActor = getSrBattleSummonActor(summonIdx);
    *gameActor.actorBattleVars = *srActor.actorBattleVars;
    *gameActor.actorTimers = *srActor.actorTimers;
    *gameActor.party10 = *srActor.party10;
    *gameActor.party34 = *srActor.party34;
    //*gameActor.weaponCtx = *srActor.weaponCtx;
    gContext.party.battleDeactivatePartyMember(0);
    gContext.party.battleDeactivatePartyMember(2);
    getActiveBattleActor(0).actorBattleVars->stateFlags &= (~(0x8));
    getActiveBattleActor(2).actorBattleVars->stateFlags &= (~(0x8));
    srLogWrite("state for disabled actor: %x", getActiveBattleActor(0).actorBattleVars->stateFlags);
}

bool SrBattleActors::isActorSummon(u8 actorIdx) {
    return actorIsSummon[actorIdx];
}



void SrBattleActors::savePartyActors() {
    for (auto partyIdx = 0; partyIdx < 3; partyIdx++) {
        savePartyActor(partyIdx);
    }
}

ActorBattleState SrBattleActors::getSrBattleActor(u8 actorIdx) {
    ActorBattleState actorState = ActorBattleState();
    if (actorIdx == 3) {
        actorState.actorBattleVars = getActorBattleVars(actorIdx);
        actorState.actorTimers = getActorTimerBlock(actorIdx);
        actorState.battleStats = &omniscientActor.battleActor.battleStats;
        actorState.activeStatuses = &omniscientActor.battleActor.activeStatuses;
        actorState.party10 = nullptr;
        actorState.party34 = nullptr;
        actorState.weaponCtx = nullptr;
        actorState.enemyData = nullptr;
        return actorState;
    }
    if (actorIdx < 3) {
        auto& partyActor = partyActors[activeParty[actorIdx]];
        actorState.actorBattleVars = &(partyActor.battleActor.gameAIState);
        actorState.actorTimers = &(partyActor.battleActor.gameTimerState);
        actorState.battleStats = &(partyActor.battleActor.battleStats);
        actorState.party10 = &(partyActor.party10);
        actorState.party34 = &(partyActor.party34);
        actorState.weaponCtx = &(partyActor.weaponCtx);
        actorState.activeStatuses = &(partyActors[activeParty[actorIdx]].battleActor.activeStatuses);
        actorState.enemyData = nullptr;
        auto weaponIdx = gContext.party.getActivePartyCharacter(actorIdx).equipment[0].equippedIdx;
        actorState.srWeapon = &(gContext.weapons.getResource(weaponIdx));
        return actorState;
    }
    actorState.actorBattleVars = &enemyActors[actorIdx - 4].battleActor.gameAIState;
    actorState.actorTimers = &enemyActors[actorIdx - 4].battleActor.gameTimerState;
    actorState.battleStats = &(enemyActors[actorIdx - 4].battleActor.battleStats);
    actorState.activeStatuses = &(partyActors[actorIdx - 4].battleActor.activeStatuses);
    actorState.party10 = nullptr;
    actorState.party34 = nullptr;
    actorState.weaponCtx = nullptr;
    actorState.enemyData = &enemyActors[actorIdx - 4].enemyData;
    return actorState;
}


ActorBattleState SrBattleActors::getSrBattleCharacterActor(u8 characterIdx) {
    ActorBattleState actorState = ActorBattleState();
    auto& partyActor = partyActors[characterIdx];
    actorState.actorBattleVars = &(partyActor.battleActor.gameAIState);
    actorState.actorTimers = &(partyActor.battleActor.gameTimerState);
    actorState.battleStats = &(partyActor.battleActor.battleStats);
    actorState.party10 = &(partyActor.party10);
    actorState.party34 = &(partyActor.party34);
    actorState.weaponCtx = &(partyActor.weaponCtx);
    actorState.activeStatuses = &(partyActors[characterIdx].battleActor.activeStatuses);
    actorState.enemyData = nullptr;
    auto weaponIdx = gContext.party.getActivePartyCharacter(characterIdx).equipment[0].equippedIdx;
    actorState.srWeapon = &(gContext.weapons.getResource(weaponIdx));
    return actorState;
}

ActorBattleState SrBattleActors::getSrBattleSummonActor(u8 summonIdx) {
    ActorBattleState actorState = ActorBattleState();
    auto& partyActor = summonActors[summonIdx];
    actorState.actorBattleVars = &(partyActor.battleActor.gameAIState);
    actorState.actorTimers = &(partyActor.battleActor.gameTimerState);
    actorState.battleStats = &(partyActor.battleActor.battleStats);
    actorState.party10 = &(partyActor.party10);
    actorState.party34 = &(partyActor.party34);
    actorState.weaponCtx = nullptr;
    actorState.activeStatuses = &(summonActors[summonIdx].battleActor.activeStatuses);
    actorState.enemyData = nullptr;
    auto weaponIdx = 0;
    actorState.srWeapon = nullptr;
    return actorState;
}

ActorBattleState SrBattleActors::getActiveBattleActor(u8 actorIdx) {
    ActorBattleState actorState = ActorBattleState();
    actorState.actorBattleVars = getActorBattleVars(actorIdx);
    actorState.actorTimers = getActorTimerBlock(actorIdx);
    if (isActorSummon(actorIdx)) {
        actorState.battleStats = &(summonActors[activeSummonIdx].battleActor.battleStats);
        actorState.activeStatuses = &(summonActors[activeSummonIdx].battleActor.activeStatuses);
        actorState.party10 = getBattleParty10(actorIdx);
        actorState.party34 = getBattleParty34(actorIdx);
        actorState.weaponCtx = nullptr;
        actorState.enemyData = nullptr;
        auto weaponIdx = 0;
        actorState.srWeapon = nullptr;
        return actorState;
    }
    if (actorIdx == 3) {
        actorState.battleStats = &omniscientActor.battleActor.battleStats;
        actorState.activeStatuses = &omniscientActor.battleActor.activeStatuses;
        actorState.party10 = nullptr;
        actorState.party34 = nullptr;
        actorState.weaponCtx = nullptr;
        actorState.enemyData = nullptr;
        actorState.weaponCtx = nullptr;
        return actorState;
    }
    if (actorIdx < 3) {
        actorState.battleStats = &(partyActors[activeParty[actorIdx]].battleActor.battleStats);
        actorState.activeStatuses = &(partyActors[activeParty[actorIdx]].battleActor.activeStatuses);
        actorState.party10 = getBattleParty10(actorIdx);
        actorState.party34 = getBattleParty34(actorIdx);
        actorState.weaponCtx = getBattleWeaponCtx(actorIdx);
        actorState.enemyData = nullptr;
        auto weaponIdx = gContext.party.getActivePartyCharacter(actorIdx).equipment[0].equippedIdx;
        actorState.srWeapon = &(gContext.weapons.getResource(weaponIdx));
        return actorState;
    }
    actorState.battleStats = &(enemyActors[actorIdx - 4].battleActor.battleStats);
    actorState.activeStatuses = &(enemyActors[actorIdx -4].battleActor.activeStatuses);
    actorState.party10 = nullptr;
    actorState.party34 = nullptr;
    actorState.weaponCtx = nullptr;
    actorState.enemyData = &enemyActors[actorIdx - 4].enemyData;
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
    return &(G_BATTLE_WEAPON_ARRAY[actorIdx]);
}

const SrBattleStat& getSrBattleStat(u8 actorID, std::string statName) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    return statMap[statName];
}

SrBattleStat& getMutableSrBattleStat(u8 actorID, std::string statName) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    return statMap[statName];
}

void setActorBattleStat(u8 actorID, std::string statName, i32 value) {
    auto& statMap = *gContext.battleActors.getActiveBattleActor(actorID).battleStats;
    auto& stat = statMap[statName];
    auto& statData = gContext.stats.getElement(statName);
    stat.statValue = (value <= statData.maxValue) ? value : statData.maxValue;
}

CharacterRecord* getCharacterRecordWithID(u8 characterID) {
    u8* byte_9ACAD4 = (u8*)0x9ACAD4;
    u16* G_BATTLE_FLAGS = (u16*)0x9A88A6;
    for (u8 characterRecordIdx = 0; characterRecordIdx < 9; ++characterRecordIdx) {
        auto characterRecord = &(CHARACTER_RECORD_ARRAY[characterRecordIdx]);
        if (*G_BATTLE_FLAGS & 0x40)
            memcpy(&byte_9ACAD4, &(CHARACTER_RECORD_ARRAY[characterRecordIdx]), sizeof(CharacterRecord));

        if (characterRecord->characterID == characterID)
            return characterRecord;
    }
    return nullptr;
}

#define DEX_NORMALIZATION (u16)75
void initializePlayerActors() {
    //DebugBreak();
    u16* G_DEX_NORMALIZATION = (u16*)0x9AAD00;
    auto partyDex = 0;
    auto partyCount = 0;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.actorPartyMask = 0;
    *G_DEX_NORMALIZATION = DEX_NORMALIZATION;
    for (auto characterID = 0; characterID < 9; characterID++) {
        bool isActive = false;
        for (auto partyIdx = 0; partyIdx < 3; partyIdx++) {
            if (characterID == G_SAVE_MAP->activeParty[partyIdx]) {
                aiBattleContext.actorPartyMask |= 1 << partyIdx;
                gContext.battleActors.initializePartyActor(partyIdx, characterID);
                isActive = true;
                break;
            }
        }
        if (!isActive) {
            gContext.battleActors.initializePartyCharacter(characterID);
        }
    }
    for (auto summonIdx = 0; summonIdx < 16; summonIdx++) {
        gContext.battleActors.initializePartySummon(summonIdx);
    }
}

void initializeEnemyActors() {
    u8* byte_9AAD2C = (u8*)0x9AAD2C;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.enemyActorsMask = 0;
    for (auto sceneRelativeIdx = 0; sceneRelativeIdx < 3; ++sceneRelativeIdx)
        byte_9AAD2C[sceneRelativeIdx] = 0;

    for (auto enemyIdx = 0; enemyIdx < 6; ++enemyIdx) {
        gContext.battleActors.initializeEnemyActor(enemyIdx);
    }
}


void setPartyStats(u8 characterID, SrPartyBattleActor& partyActor) {
    const auto& srPartyMember = gContext.party.getSrCharacter(characterID);
    setBattleStats(srPartyMember, partyActor);
}

void setBattleStats(const PartyMemberState& srPartyMember, SrPartyBattleActor& partyActor) {
    auto& actorBattleVars = partyActor.battleActor.gameAIState;
    auto& stats = srPartyMember.srPartyMember->stats;
    auto& battleStats = partyActor.battleActor.battleStats;

    for (const auto& statElement : gContext.stats.named_registry) {
        if (stats.find(statElement.first) != stats.end()) {
            battleStats[statElement.first].statValue = stats.at(statElement.first).statValue;
            battleStats[statElement.first].activeValue = battleStats[statElement.first].statValue;
        }
        else {
            battleStats[statElement.first].statValue = 0;
            battleStats[statElement.first].activeValue = 0;
        }
    }
    //Setup old acto battle vars for now
    actorBattleVars.maxHP = battleStats[StatNames::HP].statValue;
    actorBattleVars.currentHP = battleStats[StatNames::HP].statValue;
    actorBattleVars.maxMP = battleStats[StatNames::MP].statValue;
    actorBattleVars.currentMP = battleStats[StatNames::MP].statValue;
    actorBattleVars.physAtk = battleStats[StatNames::STRENGTH].statValue;
    actorBattleVars.magAtk = battleStats[StatNames::MAGIC].statValue;
    actorBattleVars.defense = 2 * battleStats[StatNames::VITALITY].statValue;
    actorBattleVars.mDefense = 2 * battleStats[StatNames::SPIRIT].statValue;
    actorBattleVars.dexterity = battleStats[StatNames::DEXTERITY].statValue;
    actorBattleVars.luck = battleStats[StatNames::LUCK].statValue;
    actorBattleVars.pEvade = battleStats[StatNames::EVADE].statValue;
    actorBattleVars.mEvade = battleStats[StatNames::MEVADE].statValue;
    partyActor.party34.currentHP = actorBattleVars.currentHP;
    partyActor.party34.currentMP = actorBattleVars.currentMP;
    if (!actorBattleVars.physAtk)
        actorBattleVars.physAtk = 1;

    partyActor.party34.maxHP = actorBattleVars.maxHP;
    partyActor.party34.maxMP = actorBattleVars.maxMP;

    if (srPartyMember.gamePartyMember->actorFlags & 8) {
        partyActor.party34.field_16 = 999;
        partyActor.party34.field_14 = 9999;
    }
    else {
        partyActor.party34.field_16 = 9999;
        partyActor.party34.field_14 = 999;
    }
    actorBattleVars.pEvade = stats["PEV"].statValue;
    actorBattleVars.mEvade = stats["MEV"].statValue;
}

void setWeaponData(u8 characterID, ActorBattleState& summonActor) {
    const auto& srPartyMember = gContext.party.getSrCharacter(characterID);
    auto& partyMember = *srPartyMember.gamePartyMember;
    auto& partyWeaponCtx = *summonActor.weaponCtx;

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

    u8 accessMaskCheck = 1;

    u16 soundID = partyMember.weaponData.normalSoundID;
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

//Fetch Enemy Battle Stats from Enemy Registry
void setEnemyStats(u8 enemyIndex, ActorBattleState& enemyActor) {
    const auto& formationEnemy = *getInBattleFormationActorData(enemyIndex);
    const auto& srEnemy = gContext.enemies.getResource(getUniqueEnemyID(formationEnemy.enemyID));
    auto& actorBattleVars = enemyActor.actorBattleVars;
    auto& stats = srEnemy.enemyStats;
    auto& battleStats = *enemyActor.battleStats;
    for (const auto& statElement : gContext.stats.named_registry) {
        if (stats.find(statElement.first) != stats.end()) {
            battleStats[statElement.first].statValue = stats.at(statElement.first).statValue;
            battleStats[statElement.first].activeValue = battleStats[statElement.first].statValue;
        }
        else {
            battleStats[statElement.first].statValue = 0;
        }
    }
    if (!actorBattleVars->physAtk)
        actorBattleVars->physAtk = 1;

    //Initialize the old memory temporarily
    actorBattleVars->maxHP = battleStats[StatNames::HP].statValue;
    actorBattleVars->currentHP = battleStats[StatNames::HP].statValue;
    actorBattleVars->maxMP = battleStats[StatNames::MP].statValue;
    actorBattleVars->currentMP = battleStats[StatNames::MP].statValue;
    actorBattleVars->physAtk = battleStats[StatNames::STRENGTH].statValue;
    actorBattleVars->magAtk = battleStats[StatNames::MAGIC].statValue;
    actorBattleVars->defense = 2 * battleStats[StatNames::VITALITY].statValue;
    actorBattleVars->mDefense = 2 * battleStats[StatNames::SPIRIT].statValue;
    actorBattleVars->dexterity = battleStats[StatNames::DEXTERITY].statValue;
    actorBattleVars->luck = battleStats[StatNames::LUCK].statValue;
    actorBattleVars->pEvade = battleStats[StatNames::EVADE].statValue;
    actorBattleVars->mEvade = battleStats[StatNames::MEVADE].statValue;
}


ActorBattleState getActiveBattleActor(u8 actorIdx) {
    return gContext.battleActors.getActiveBattleActor(actorIdx);
}


SISTERRAY_API SrBattleStat* getBattleActorStat(ActorBattleState* battleState, const char* statName) {
    auto& battleStats = *battleState->battleStats;
    return &(battleStats[std::string(statName)]);
}
