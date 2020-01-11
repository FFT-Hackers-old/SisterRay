#include "battle_actors.h"
#include "../impl.h"

#define ACTOR_COUNT 10
#define ENEMY_ACTOR_COUNT 6
#define CHARACTER_COUNT 10

SrBattleActors::SrBattleActors() : SrNamedResourceRegistry<SrBattleActor, std::string>() {
    for (u8 actorIdx = 0; actorIdx < CHARACTER_COUNT; actorIdx++) {
        partyActors[actorIdx] = SrPartyBattleActor();
    }
    for (u8 enemyActorIdx = 0; enemyActorIdx < ENEMY_ACTOR_COUNT; enemyActorIdx++) {
        enemyActors[enemyActorIdx] = SrBattleActor();
    }
}

void SrBattleActors::setPartyMemberActive(u8 partyIdx, u8 characterID) {
    if (partyIdx < 3)
        activeParty[partyIdx] = characterID
}

void SrBattleActors::swapPartyActors(u8 partyIdx, u8 newCharacterID) {
    auto& partyMember = getActiveBattleActor(partyIdx);
    partyActors[activeParty[partyIdx]] = *partyMember.actorBattleVars;
    *(partyMember.actorBattleVars) = partyActors[newCharacterID];
    setPartyMemberActive(partyIdx, newCharacterID);
}

ActorBattleState SrBattleActors::getActiveBattleActor(u8 actorIdx) {
    ActorBattleState actorState = ActorBattleState();
    actorState.actorBattleVars = getActorBattleVars(actorIdx);
    actorState.actorTimers = getActorTimerBlock(actorIdx);
    if (actorIdx < 3) {
        actorState.battleStats = &(partyActors[activeParty[actorIdx]].battleActor.battleStats);
        actorState.party10 = getBattleParty10(actorIdx);
        actorState.party34 = getBattleParty10(actorIdx);
        actorState.weaponCtx = getBattleWeaponCtx(actorIdx);
        return actorState;
    }
    actorState.battleStats = &(enemyActors[actorIdx].battleActor.battleStats);
    actorState.party10 = nullptr;
    actorState.party34 = nullptr;
    return actorState;
}

u8* SrBattleActors::getActorRandomVarBuffer(u8 actorIdx) {
    return &(actorScriptVariables[actorIdx].buffer);
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

void initializePlayerActors() {

    auto partyDex = 0;
    auto partyCount = 0;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.actorPartyMask = 0;
    for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
        u8 characterID = G_SAVEMAP_PARTY[partyIdx];
        auto activePartyMember = getActivePartyMember(partyIdx);
        auto partyActor = gContext.battleActors.getActiveBattleActor(partyIdx);
        auto& partyMember = gContext.party.getActivePartyMember(partyIdx);
        gContext.battleActors.setPartyMemberActive(partyIdx, characterID);
        auto characterRecord = getCharacterRecordWithID(characterID);
        if (characterRecord = nullptr)
            return;

        aiBattleContext.actorPartyMask |= 1 << partyIdx;
        initializePartyActor(partyMember, activePartyMember, partyActor, characterRecord);
    }
    if (partyCount)
        G_DEX_NORMALIZATION = (unsigned int)((partyDex + partyCount - 1) / partyCount) + 50;
}

void initializePartyActor(PartyMemberState& srPartyMember, ActorBattleState& partyActor, CharacterRecord& characterRecord) {
    auto actorTimers = partyActor.actorTimers;
    auto actorBattleVars = partyActor.actorBattleVars;
    auto party34 = partyActor.party34;
    auto party10 = partyActor.party10;
    auto gamePartyMember = srPartyMember.gamePartyMember;
    auto partyWeaponCtx = partyActor.weaponCtx;

    party10->characterID = -1;
    v5 = 1;
    actorTimers->unkActorFlags = 1;
    actorTimers->field_C = -1;
    actorTimers->field_F = -1;
    party10->characterID = characterID;
    actorBattleVars->index = characterID;
    actorBattleVars->charID = characterID + 0x10;
    actorBattleVars->level = characterRecord->level;
    actorBattleVars->idleAnimHolder = 0;
    LOBYTE(actorBattleVars->ApValue) = 8;
    actorBattleVars->damageAnimID = 5;
    actorBattleVars->actorFlags = 8;
    party34->characterRecord = characterRecord;
    if (!(characterRecord->characterRow & 1))
        actorBattleVars->actorFlags |= 0x40u;

    actorBattleVars->currentHP = gamePartyMember->currentHP;
    actorBattleVars->currentMP = gamePartyMember->currentMP;
    party34->currentHP = actorBattleVars->currentHP;
    party34->currentMP = actorBattleVars->currentMP;
    actorBattleVars->dexterity = gamePartyMember->speed;
    actorBattleVars->luck = gamePartyMember->luck;
    actorBattleVars->maxHP = gamePartyMember->maxHP;
    actorBattleVars->maxMP = gamePartyMember->maxMP;
    actorBattleVars->physAtk = gamePartyMember->physAttack;
    actorBattleVars->magAtk = gamePartyMember->magAttack;
    actorBattleVars->defense = gamePartyMember->physDefense;
    actorBattleVars->mdefense = gamePartyMember->magDefense;

    if (!actorBattleVars->physAtk)
        actorBattleVars->physAtk = 1;
    party34->maxHP = actorBattleVars->maxHP;
    party34->maxMP = actorBattleVars->maxMP;
    if (gamePartyMember->actorFlags & 8) {
        party34->field_16 = 999;
        party34->field_14 = 9999;
    }
    else {
        party34->field_16 = 9999;
        party34->field_14 = 999;
    }
    actorBattleVars->backDamageMultiplier = 16;
    actorBattleVars->statusMasks = characterRecord->out_of_battle_status & 0x30;
    actorBattleVars->initStatusMasks = 0;
    actorTimers->statusImmuneMask = gamePartyMember->immuneStatusesMask;
    partyWeaponCtx->targetFlags = gamePartyMember->equippedWeaponData.targetFlags;
    partyWeaponCtx->attackEffectID = gamePartyMember->equippedWeaponData.attackEffectID;
    partyWeaponCtx->damageCalculation = gamePartyMember->equippedWeaponData.damageCalculation;
    partyWeaponCtx->weaponHitRate = gamePartyMember->equippedWeaponData.weaponHitRate;
    partyWeaponCtx->impactEffectID = gamePartyMember->equippedWeaponData.impactEffectID;
    partyWeaponCtx->criticalRate = gamePartyMember->equippedWeaponData.criticalRate;
    partyWeaponCtx->attackElementMask = gamePartyMember->attackElementsMask | gamePartyMember->equippedWeaponData.attackElementsMask;
    partyWeaponCtx->cameraMovemennt = gamePartyMember->equippedWeaponData.cameraMovement;
    partyWeaponCtx->specialAttackFlags = gamePartyMember->equippedWeaponData.specialAttackFlags;
    partyWeaponCtx->attackStatusMask = gamePartyMember->attackStatusesMask;
    const auto& armor = gContext.armors.getResource()
    actorAiDataPtr->pEvade = v0->evade;
    actorAiDataPtr->mEvade = v0->magicEvade;
    handleAccessorySpecialEffect(partyActorID, (unsigned __int8)characterRecordPtr->equipped_accessory);
    actorAiDataPtr->gilStolen = 0;
    actorAiDataPtr->itemStolen = -1;
    u16 soundId; = gamePartyMember->weaponData.normal_sound_id;
    for (u8 soundIdx = 0; soundIdx < 3; ++soundIdx) {
        v4 = *((u8*)&gamePartyMember->equippedWeaponData.normalSoundID + soundIdx);
        if ((unsigned __int8)v5 & gamePartyMember->equippedWeaponData.soundAccessMask)
            v4 |= 0x100u;
        partyWeaponCtx->soundData[soundIdx] = v4;
        v5 *= 2;
    }
    actorTimers->unkActorFlags &= 0xFDu;
    if (gamePartyMember->actorFlags & 4)
        *weaponCtx &= 0xDFu;
    if (!(*weaponCtx & 0x20))
        actorTimers->unkActorFlags |= 2u;
    actorTimers->CharATBGauge = 0;
    gamePartyMember->unknownDivisor = 0;
    gamePartyMember->atbTimer = 0;
    gamePartyMember->barrierGauge = 0;
    gamePartyMember->mBarrierGauge = 0;
    gamePartyMember->limitGuage = 0;
    gamePartyMember->activeLimitLevel = 1;
    party34->field_5 = -1;
    if (characterID < 9) {
        v3 = (unsigned __int8)characterRecord->active_limit_level;
        if (v3 >= 1 && v3 <= 4)
        {
            v1 = v3 - 1;
            party34->limitBreakBar = (unsigned __int8)characterRecord->limit_bar_progress;
            party34->field_A = party34->limitBreakBar;
            party34->field_5 = v1;
            party34->field_1C = dword_99CE38[14 * characterID + v1];
            gamePartyMember->limitGuage = G_BATTLE_PARTY_34_ARRAY[partyIdx].limitBreakBar << 8;
            gamePartyMember->activeLimitLevel = characterRecord->active_limit_level;
            setLearnedLimits(
                characterID,
                (unsigned __int16)characterRecord->learnedLimits,
                (int)gamePartyMember->enabledLimitBytes);
        }
    }
    actorBattleVars->actorFlags |= 8u;
    if (!actorBattleVars->currentHP)
    {
        v2 = actorBattleVars->statusMasks;
        LOBYTE(v2) = actorBattleVars->statusMasks | 1;
        actorBattleVars->statusMasks = v2;
    }
    setCommandTargeting(partyIdx);
    sub_437370(partyIdx);
    if (party34->limitBreakBar == 255) {
        sub_4375CE(partyIdx);
        G_ACTOR_TIMER_ARRAY[partyIdx].field_8 &= 0xFFFEu;
    }
    if (actorBattleVars->statusMasks)
        sub_5C7DB0(partyIdx);
    party34->learnedESkillsMask = getLearnedEnemySkills(characterRecord);
    party34->knownEnemySkills = party34->learnedESkillsMask;
    partyDex += characterRecord->dexterity;
    ++partyCount;
    break;
}

void setPartyStats(const PartyMemberState& srPartyMember, ActorBattleState& partyActor) {
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

void initializeEnemyActors() {

}
