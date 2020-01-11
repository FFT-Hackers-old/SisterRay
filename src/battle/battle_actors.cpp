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
    ArmorData* v0; // ST1C_4
    int v1; // ST14_4
    int v2; // eax
    signed int v3; // [esp+8h] [ebp-44h]
    __int16 v4; // [esp+Ch] [ebp-40h]
    char v5; // [esp+14h] [ebp-38h]
    BYTE* someStruct; // [esp+34h] [ebp-18h]
    int partyCount; // [esp+38h] [ebp-14h]
    signed int partyActorID; // [esp+44h] [ebp-8h]
    int partyDex; // [esp+48h] [ebp-4h]

    partyDex = 0;
    partyCount = 0;
    auto& aiBattleContext = *AI_BATTLE_CONTEXT;
    aiBattleContext.actorPartyMask = 0;
    for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
        u8 characterID = G_SAVEMAP_PARTY[partyIdx];
        auto activePartyMember = getActivePartyMember(partyIdx);
        auto partyActor = gContext.battleActor
            s.getActiveBattleActor(partyIdx);
        gContext.battleActors.setPartyMemberActive(partyIdx, characterID);
        someStruct = (BYTE*)&G_BATTLE_WEAPON_DATA[24 * partyIdx];

        auto characterRecord = getCharacterRecordWithID(characterID);
        if (characterRecord = nullptr)
            return;

        aiBattleContext.actorPartyMask |= 1 << partyIdx;
        initializePartyActor(activePartyMember, partyActor, characterRecord);
    }
    if (partyCount)
        G_DEX_NORMALIZATION = (unsigned int)((partyDex + partyCount - 1) / partyCount) + 50;
}

void initializePartyActor(PartyMember& srPartyMember, ActorBattleState& partyActor, CharacterRecord& characterRecord) {
    auto actorTimers = partyActor.actorTimers;
    auto actorBattleVars = partyActor.actorBattleVars;
    auto party34 = partyActor.party34;
    auto party10 = partyActor.party10;
    auto activePartyMember = &(srPartyMember);

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
    actorBattleVars->currentHP = activePartyMember->currentHP;
    actorBattleVars->currentMP = activePartyMember->currentMP;
    party34->currentHP = actorBattleVars->currentHP;
    party34->currentMP = actorBattleVars->currentMP;
    actorBattleVars->dexterity = activePartyMember->speed;
    actorBattleVars->luck = activePartyMember->luck;
    actorBattleVars->maxHP = activePartyMember->maxHP;
    actorBattleVars->maxMP = activePartyMember->maxMP;
    actorBattleVars->physAtk = activePartyMember->physAttack;
    actorBattleVars->magAtk = activePartyMember->magAttack;
    actorBattleVars->defense = activePartyMember->physDefense;
    actorBattleVars->mdefense = activePartyMember->magDefense;
    if (!actorBattleVars->physAtk)
        actorBattleVars->physAtk = 1;
    party34->maxHP = actorBattleVars->maxHP;
    party34->maxMP = actorBattleVars->maxMP;
    if (activePartyMember->actorFlags & 8) {
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
    actorTimers->statusImmuneMask = activePartyMember->immuneStatusesMask;
    *someStruct = activePartyMember->equippedWeaponData.targetFlags;
    someStruct[1] = activePartyMember->equippedWeaponData.attackEffectID;
    someStruct[2] = activePartyMember->equippedWeaponData.damageCalculation;
    someStruct[3] = activePartyMember->equippedWeaponData.weaponHitRate;
    someStruct[4] = activePartyMember->equippedWeaponData.impactEffectID;
    someStruct[5] = activePartyMember->equippedWeaponData.criticalRate;
    *((_WORD*)someStruct + 7) = activePartyMember->attackElementsMask | activePartyMember->equippedWeaponData.attackElementsMask;
    *((_WORD*)someStruct + 8) = activePartyMember->equippedWeaponData.cameraMovement;
    *((_WORD*)someStruct + 9) = activePartyMember->equippedWeaponData.specialAttackFlags;
    *((_DWORD*)someStruct + 5) = activePartyMember->attackStatusesMask;
    v0 = &gArmorDataTable[(unsigned __int8)characterRecord->equipped_armor];
    actorBattleVars->pEvade = v0->evade;
    actorBattleVars->mEvade = v0->magicEvade;
    handleAccessorySpecialEffect(partyIdx, (unsigned __int8)characterRecord->equipped_accessory);
    actorBattleVars->gilStolen = 0;
    actorBattleVars->itemStolen = -1;
    for (u8 j = 0; j < 3; ++j) {
        v4 = *((unsigned __int8*)&activePartyMember->equippedWeaponData.normalSoundID + j);
        if ((unsigned __int8)v5 & activePartyMember->equippedWeaponData.soundAccessMask)
            v4 |= 0x100u;
        *(_WORD*)&someStruct[2 * j + 8] = v4;
        v5 *= 2;
    }
    actorTimers->unkActorFlags &= 0xFDu;
    if (activePartyMember->actorFlags & 4)
        *someStruct &= 0xDFu;
    if (!(*someStruct & 0x20))
        actorTimers->unkActorFlags |= 2u;
    actorTimers->CharATBGauge = 0;
    activePartyMember->unknownDivisor = 0;
    activePartyMember->atbTimer = 0;
    activePartyMember->barrierGauge = 0;
    activePartyMember->mBarrierGauge = 0;
    activePartyMember->limitGuage = 0;
    activePartyMember->activeLimitLevel = 1;
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
            activePartyMember->limitGuage = G_BATTLE_PARTY_34_ARRAY[partyIdx].limitBreakBar << 8;
            activePartyMember->activeLimitLevel = characterRecord->active_limit_level;
            setLearnedLimits(
                characterID,
                (unsigned __int16)characterRecord->learnedLimits,
                (int)activePartyMember->enabledLimitBytes);
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

void initializeEnemyActors() {

}
