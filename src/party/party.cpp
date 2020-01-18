#include "party.h"
#include "../impl.h"
#include "stat_calculation.h"
#include "stat_names.h"

#define CHARACTER_COUNT 0xA
#define PARTY_COUNT 3

void initParty() {
    gContext.party = SrPartyMembers(CHARACTER_COUNT);
}

SrPartyMembers::SrPartyMembers(u8 characterCount) {
    srLogWrite("initializing Sister Ray Party for %lu characters", characterCount);
    for (u8 partyIdx = 0; partyIdx < characterCount; partyIdx++) {
        SrPartyData partyData = {};
        auto& stats = partyData.playerStats;
        SrActorStat stat = { 1, 1 };
        for (const auto& element : gContext.stats.named_registry) {
            stats[element.first] = stat;
        }
        srLogWrite("Initialized party member with %lu stats", partyData.playerStats.size());
        partyMembers[partyIdx] = partyData;
    }
}


SrCharacterData& SrPartyMembers::getActivePartyCharacter(u8 partyIdx) {
    if (partyIdx < 3) {
        return gContext.characters.getResource(activeParty[partyIdx]);
    }
    return gContext.characters.getResource(0);
}

void SrPartyMembers::addAutoAction(u32 partyIndex, const SrAutoAction& action) {
    auto& autoActons = getSrPartyMember(partyIndex).srPartyMember->actorAutoActions;
    for (auto& actionSlot : autoActons) {
        if (actionSlot.dispatchType == AUTOACT_NO_ACTION) {
            actionSlot = action;
        }
    }
}

/* clear all data arrays prior to recalculation based on current equipment configuration*/
void SrPartyMembers::clearActions(u32 partyIndex) {
    auto& actor = getSrPartyMember(partyIndex);
    auto& magicArray = actor.srPartyMember->actorMagics;
    auto& summonArray = actor.srPartyMember->actorSummons;
    auto& eSkillArray = actor.srPartyMember->actorEnemySkills;
    auto& autoActions = actor.srPartyMember->actorAutoActions;
    clearActionArray<MAGIC_COUNT>(magicArray);
    clearActionArray<SUMMON_COUNT>(summonArray);
    clearActionArray<ESKILL_COUNT>(eSkillArray);
    clearAutoActionArray<AUTO_ACTION_COUNT>(autoActions);
    clearCommandArray(partyIndex);
}


PartyMemberState SrPartyMembers::getActivePartyMember(u8 partyIdx) {
    auto characterID = activeParty[partyIdx];
    PartyMemberState ret = { getGamePartyMember(partyIdx), &(partyMembers[characterID]) };
    return ret;
}

PartyMemberState SrPartyMembers::getSrPartyMember(u8 partyIdx) {
    auto characterID = activeParty[partyIdx];
    PartyMemberState ret = { &(gamePartyMembers[characterID]), &(partyMembers[characterID]) };
    return ret;
}

/*This method enables actions*/
void SrPartyMembers::handleMateriaActorUpdates(u8 partyIdx, const std::vector<MateriaInventoryEntry>& equippedMaterias) {
    if (partyIdx > 3)
        return;

    bool magicEnabled = false;
    bool summonEnabled = false;
    for (auto materia : equippedMaterias) {
        if (materia.item_id == 0xFFFF)
            continue;
        if ((getMateriaTopType(materia.item_id) == 0x9) || (getMateriaTopType(materia.item_id) == 0xA))
            magicEnabled = true;
        if ((getMateriaTopType(materia.item_id) == 0xB) || (getMateriaTopType(materia.item_id) == 0xC)) {
            summonEnabled = true;
        }
    }
    enableDefaultCommands(partyIdx, magicEnabled, summonEnabled);

    for (auto it = std::begin(equippedMaterias); it != std::end(equippedMaterias); ++it) {
        auto materia = *it;
        if (materia.item_id == 0xFFFF)
            continue;
        u8 maxLevel = 1;
        auto materiaLevel = getMateriaLevel(materia, &maxLevel);
        EnableAbilitiesEvent enableActionEvent = { partyIdx, materia, gContext.materias.getResource(materia.item_id).gameMateria, materiaLevel };
        auto topkey = getTopKey(getMateriaTopType(materia.item_id));
        auto subkey = getSubKey(getMateriaSubType(materia.item_id));
        std::vector<SrEventContext> dispatchContexts = { topkey, subkey };
        gContext.eventBus.dispatch(ENABLE_ACTIONS, &enableActionEvent, dispatchContexts);
    }
    gamePartyMembers[activeParty[partyIdx]].commandColumns = getCommandRows(partyIdx);
}

void SrPartyMembers::battleActivatePartyMember(u8 partyIdx) {
    auto& srMember = getSrPartyMember(partyIdx);
    auto& activeMember = getActivePartyMember(partyIdx);

    srLogWrite("ACTIVATING PARTY MEMBER at %p", &activeMember);
    srLogWrite("Max HP: %d", srMember.gamePartyMember->maxHP);
    srLogWrite("Strength: %d", srMember.gamePartyMember->strength);
    *activeMember.gamePartyMember = *srMember.gamePartyMember;
}

void SrPartyMembers::battleSavePartyMember(u8 partyIdx) {
    auto& srMember = getSrPartyMember(partyIdx);
    auto& activeMember = getActivePartyMember(partyIdx);

    *srMember.gamePartyMember = *activeMember.gamePartyMember;
}


void SrPartyMembers::setPartyMemberActive(u8 partyIdx, u8 newCharacterID) {
    if (partyIdx < 3) {
        activeParty[partyIdx] = newCharacterID;
    }
}


void SrPartyMembers::swapPartyMembers(u8 partyIdx, u8 newCharacterID) {
    battleSavePartyMember(partyIdx);
    activeParty[partyIdx] = newCharacterID;
    battleActivatePartyMember(partyIdx);
}

void SrPartyMembers::initPartyBattleFields(u8 partyIdx, const ActorBattleState& actorState) {
    auto& partyMember = *getSrPartyMember(partyIdx).gamePartyMember;
    partyMember.unknownDivisor = 0;
    partyMember.atbTimer = 0;
    partyMember.barrierGauge = 0;
    partyMember.mBarrierGauge = 0;
    partyMember.limitGuage = 0;
    partyMember.activeLimitLevel = 1;

    partyMember.limitGuage = actorState.party34->limitBar << 8;
    const auto& characterRecord = gContext.characters.getResource(activeParty[partyIdx]);
    partyMember.activeLimitLevel = characterRecord.gameCharacter->activeLimitLevel;

    partyMember.commandColumns = 1;

    for (u8 commandSlotIdx = 0; commandSlotIdx < 16; ++commandSlotIdx) {
        u8 finalTargetingData = 0xFF;
        u8 commandID = partyMember.enabledCommandArray[commandSlotIdx].commandID;
        auto& enabledCommand = partyMember.enabledCommandArray[commandSlotIdx];
        if (commandID != 0xFF) {
            finalTargetingData = getCommand(commandID).gameCommand.targetingFlags;
            if (finalTargetingData == 0xFF)
                finalTargetingData = actorState.weaponCtx->targetFlags;

            if (commandID >= 0x18u && commandID <= 0x1Bu)
                enabledCommand.allCount = -1;

            if (enabledCommand.cursorCommandType == 7) {
                if (actorState.actorTimers->unkActorFlags & 2)
                    enabledCommand.cursorCommandType = 0;
                if (enabledCommand.allCount && enabledCommand.commandID != 25) {
                    finalTargetingData |= 0xC;
                }
                if (commandID == 5 || commandID == 17) {
                    finalTargetingData |= 0x10;
                    if (enabledCommand.allCount)
                        enabledCommand.cursorCommandType = 0;
                }
            }
            partyMember.commandColumns = commandSlotIdx / 4 + 1;
        }
        enabledCommand.targetingData = finalTargetingData;
    }
    battleActivatePartyMember(partyIdx);
}

void SrPartyMembers::recalculatePartyMember(u8 partyIdx) {
    auto characterID = G_SAVE_MAP->activeParty[partyIdx];
    setPartyMemberActive(partyIdx, characterID);
    auto characterName = getCharacterName(characterID);
    auto weaponMaterias = gContext.characters.getElement(characterName).wpnMaterias;
    auto armorMaterias = gContext.characters.getElement(characterName).armMaterias;

    std::vector<MateriaInventoryEntry> equippedMaterias = std::vector<MateriaInventoryEntry>();
    equippedMaterias.resize(weaponMaterias.max_size() + armorMaterias.max_size());

    std::vector<MateriaInventoryEntry> wpnVector = std::vector<MateriaInventoryEntry>();
    wpnVector.resize(weaponMaterias.max_size());
    std::vector<MateriaInventoryEntry> armVector = std::vector<MateriaInventoryEntry>();
    armVector.resize(armorMaterias.max_size());

    std::copy(begin(weaponMaterias), end(weaponMaterias), begin(equippedMaterias));
    std::copy(begin(armorMaterias), end(armorMaterias), begin(equippedMaterias) + weaponMaterias.size());
    std::copy(begin(weaponMaterias), end(weaponMaterias), begin(wpnVector));
    std::copy(begin(armorMaterias), end(armorMaterias), begin(armVector));

    clearActions(partyIdx);
    handleMateriaActorUpdates(partyIdx, equippedMaterias); //Enable spells, counters, commands, and fill out stat boosts based on materia stuff
    applyLinkedMateriaModifiers(partyIdx, wpnVector, SR_GEAR_WEAPON);
    applyLinkedMateriaModifiers(partyIdx, armVector, SR_GEAR_ARMOR);

    StatBoostModifiers statModifiers = StatBoostModifiers();
    //Add all stat modifiers from weapons to the modification object, for determining the base value of that stat
    auto& characterRecord = gContext.characters.getResource(characterID);
    const auto& weapon = gContext.weapons.getResource(characterRecord.equippedWeapon);
    addStatBoosts(statModifiers, weapon.equipEffects);
    const auto& armor = gContext.armors.getResource(characterRecord.equippedArmor);
    addStatBoosts(statModifiers, armor.equipEffects);
    const auto& accessory = gContext.accessories.getResource(characterRecord.equippedAccessory);
    addStatBoosts(statModifiers, accessory.equipEffects);
    for (auto equippedMateria : equippedMaterias) {
        const auto& materia = gContext.materias.getResource(equippedMateria.item_id);
        addStatBoosts(statModifiers, materia.equipEffects);
    }

    auto partyMember = getSrPartyMember(partyIdx);
    calculateActorStats(*partyMember.srPartyMember, *getPartyActorCharacterRecord(partyIdx), statModifiers);
    auto& gamePartyMember = *partyMember.gamePartyMember;
    auto& srPartyMember = *partyMember.srPartyMember;
    //TODO after all references are removed kill these copies
    gamePartyMember.maxHP = srPartyMember.playerStats[StatNames::HP].statValue;
    gamePartyMember.maxMP = srPartyMember.playerStats[StatNames::MP].statValue;
    gamePartyMember.physAttack = srPartyMember.playerStats[StatNames::WEAPON_ATTACK].statValue;
    gamePartyMember.physDefense = srPartyMember.playerStats[StatNames::ARMOR_DEFENSE].statValue;
    gamePartyMember.magAttack = srPartyMember.playerStats[StatNames::WEAPON_MAGIC].statValue;
    gamePartyMember.magDefense = srPartyMember.playerStats[StatNames::ARMOR_MDEFENSE].statValue;
    gamePartyMember.strength = srPartyMember.playerStats[StatNames::STRENGTH].statValue;
    gamePartyMember.vitality = srPartyMember.playerStats[StatNames::VITALITY].statValue;
    gamePartyMember.magic = srPartyMember.playerStats[StatNames::MAGIC].statValue;
    gamePartyMember.spirit = srPartyMember.playerStats[StatNames::SPIRIT].statValue;
    gamePartyMember.speed = srPartyMember.playerStats[StatNames::DEXTERITY].statValue;
    gamePartyMember.luck = srPartyMember.playerStats[StatNames::LUCK].statValue;
    battleActivatePartyMember(partyIdx);
}


PartyMemberState getSrPartyMember(u8 partyIdx) {
    return gContext.party.getSrPartyMember(partyIdx);
}

PartyMemberState getActivePartyMember(u8 partyIdx) {
    return gContext.party.getActivePartyMember(partyIdx);
}

u8 getCommandRows(u8 partyIdx) {
    const auto commands = getSrPartyMember(partyIdx).gamePartyMember->enabledCommandArray;
    u8 count = 0;
    for (auto idx = 0; idx < 16; idx++) {
        if (commands[idx].commandID != 0xFF) {
            count++;
        }
    }
    if (count <= 4)
        return 1;
    return (count / 4) + 1;
}

/*Applys modifiers when support materia are paired with others*/
void applyLinkedMateriaModifiers(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType) {
    for (auto pairIndex = 0; pairIndex < 8; pairIndex += 2) {
        auto slots = getMateriaSlots(partyIndex, gearType);
        auto leftSlot = slots[pairIndex];
        auto rightSlot = slots[pairIndex];

        if (!slotsAreLinked(leftSlot, rightSlot)) {
            return;
        }

        auto& leftMateria = equippedMaterias[pairIndex];
        auto& rightMateria = equippedMaterias[pairIndex + 1];
        if (getMateriaTopType(leftMateria.item_id) == 5) {
            dispatchSupportHandlers(partyIndex, leftMateria, rightMateria, gearType);
        }
        if (getMateriaTopType(rightMateria.item_id) == 5) {
            dispatchSupportHandlers(partyIndex, rightMateria, leftMateria, gearType);
        }
    }
}

u8* getMateriaSlots(u8 partyIdx, SrGearType gearType) {
    u8 characterRecordArrayIndex = getCharacterRecordIndex(partyIdx);

    auto kernelObjectID = getEquippedGear(characterRecordArrayIndex, gearType);

    if (gearType == SR_GEAR_WEAPON) {
        auto materiaSlots = &(gContext.weapons.getResource(kernelObjectID).gameWeapon.materia_slots[0]);
        return materiaSlots;
    }
    else if (gearType == SR_GEAR_ARMOR) {
        auto materiaSlots = &(gContext.armors.getResource(kernelObjectID).gameArmor.materia_slots[0]);
        return materiaSlots;
    }
    return nullptr;
}

bool slotsAreLinked(u8 leftSlot, u8 rightSlot) {
    if (leftSlot == SLOT_RIGHT_LINKED && rightSlot == SLOT_LEFT_LINKED)
        return true;
    if (leftSlot == SLOT_RIGHT_LINKED_2 && rightSlot == SLOT_LEFT_LINKED_2)
        return true;
    return false;
}

/*Dispatch support materia handlers, keyed on contexts corresponding to the type and subtype of the paired materia*/
void dispatchSupportHandlers(u8 partyIndex, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGearType gearType) {
    u8 supportMax;
    u8 pairedMax;
    auto supportLevel = getMateriaLevel(supportMateria, &supportMax);
    auto pairedLevel = getMateriaLevel(pairedMateria, &pairedMax);
    ApplySupportEvent applySupportEvent = {
        partyIndex,
        supportMateria,
        supportLevel,
        gContext.materias.getResource(supportMateria.item_id).gameMateria,
        pairedMateria,
        pairedLevel,
        gContext.materias.getResource(pairedMateria.item_id).gameMateria,
        gearType
    };

    auto topkey = getTopKey(getMateriaTopType(pairedMateria.item_id));
    auto subkey = getSubKey(getMateriaSubType(pairedMateria.item_id));
    std::vector<SrEventContext> dispatchContexts = { topkey, subkey };
    gContext.eventBus.dispatch(APPLY_SUPPORT, (void*)&applySupportEvent, dispatchContexts);
}

void srUpdatePartyMember(u8 partyIndex) {
    recalculateBaseStats(partyIndex);
    recalculateDerivedStats(partyIndex); //This function will be replaced with an sr version to correctly init the new command fields
    updateMiscPartyStats();
}

/*Handler handle updating active commands*/
void srRecalculateDerivedStats(u8 partyIdx) {
    gContext.party.recalculatePartyMember(partyIdx);
}

/*determine whether or not commands are enabled from resizeable SR arrays*/
void updateCommandsActive(u8 partyIndex, i32 commandType) {
    if ((partyIndex < 3) && (1 << commandType) & G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask) {
        i32 statusMask = gAiActorVariables[partyIndex].statusMask;
        G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask &= ~(u8)(1 << commandType);
        switch (commandType) {
            case 0: {
                updateCommands(partyIndex, statusMask);
                break;
            }
            case 1: {
                G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask &= 0xDFu;// Unset Bit 0x20
                if (!updateMagicCommand(partyIndex, statusMask)) {
                    G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask |= 0x20;
                }
                break;
            }
            case 2: {
                G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask &= 0xBFu;// Unset bit 0x40
                if (!updateSummonCommand(partyIndex, statusMask))
                    G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask |= 0x40;
                break;
            }
            case 3: {
                G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask &= 0x7Fu;// unset bit 0x80
                if (!updateESkillCommand(partyIndex, statusMask))
                    G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask |= 0x80;
                break;
            }
            default:
                return;
        }
    }
}

/*Probably a good place to allow callbacks to hook into*/
void updateCommands(i32 partyIndex, i16 statusMask) {
    i32 result; // eax
    auto enabledCommands = getSrPartyMember(partyIndex).gamePartyMember->enabledCommandArray;
    u8* byte_DC3BA0 = (u8*)(0xDC3BA0);
    enabledCommands[0].commandFlags = 0;
    /*Deals with enabling limit commands. Instead we will change how limit breaks work
    /*if (((1 << partyIndex) & (u16)word_9A889E) && !(statusMask & 0x800) || (word_9AB0CC & 8)) {
        enabledCommands->commandID = CMD_LIMIT;       // Enable Limit Command
        enabledCommands->cursorActionID = byte_DB9630;
        enabledCommands->targetingData = byte_DB9631;
        result = (int)PartyDataArray[partyIndex].enabledCommands;
        enabledCommands->supportMatFlags = 0;
    }
    else if (enabledCommands->allCount && !(statusMask & 0x800)) {
        enabledCommands->commandID = byte_9AEAD0[6 * partyIndex];
        enabledCommands->cursorActionID = byte_9AEAD1[6 * partyIndex];
        enabledCommands->targetingData = byte_9AEAD2[6 * partyIndex];
        result = 6 * partyIndex;
        enabledCommands->supportMatFlags = byte_9AEAD5[6 * partyIndex];
    }
    else {
        enabledCommands->commandID = byte_9AEAB8[6 * partyIndex];
        enabledCommands->cursorActionID = byte_9AEAB9[6 * partyIndex];
        enabledCommands->targetingData = byte_9AEABA[6 * partyIndex];
        result = 6 * partyIndex;
        enabledCommands->supportMatFlags = byte_9AEABD[6 * partyIndex];
    }
    /*Deals with enabling the Limit break command*/
    for (auto enabledSlotIdx = 1; enabledSlotIdx < 16; ++enabledSlotIdx) {
        u16 commandID = enabledCommands[enabledSlotIdx].commandID;
        if (commandID != 255) {
            u8 commandFlags = enabledCommands[enabledSlotIdx].commandFlags & 0xED;
            switch (commandID) {
                case 2:
                case 21: {
                    updateCommandsActive(partyIndex, 1);
                    if (G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask & 0x20) {
                        srLogWrite("A:Flagging command %i inactive", commandID);
                        commandFlags |= 2u;
                    }
                    break;
                }
                case 3:
                case 22: {
                    updateCommandsActive(partyIndex, 2);
                    if (G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask & 0x40) {
                        commandFlags |= 2u;
                    }
                    break;
                }
                case 4:
                case 23: {
                    if (*byte_DC3BA0 & 0x20) {
                        commandFlags |= 2u;
                    }
                    break;
                }
                case 5:
                case 17: {
                    if (!enabledCommands[enabledSlotIdx].allCount)
                        enabledCommands[enabledSlotIdx].cursorCommandType = 7;
                    break;
                }
                case 13: {
                    updateCommandsActive(partyIndex, 3);
                    if (G_ACTOR_TIMER_ARRAY[partyIndex].activeCommandsMask & 0x80) {
                        commandFlags |= 2u;
                    }
                    break;
                }
                default:
                    break;
            }
            if ((statusMask & 0x800) != 0) { //disable commands when you're in the frog status
                switch (commandID) {
                    case 2:
                    case 4:
                    case 21:
                    case 23:
                        break;
                    default: {
                        commandFlags |= 2u;
                        break;
                    }
                }
            }
            if ((enabledCommands[enabledSlotIdx].targetingData & 8) && !enabledCommands[enabledSlotIdx].allCount) {
                enabledCommands[enabledSlotIdx].targetingData &= 0xF3u;
            }
            if ((enabledCommands[enabledSlotIdx].targetingData & 0xC) == 12)
                commandFlags |= 0x10u;

            enabledCommands[enabledSlotIdx].commandFlags = commandFlags;
        }
        result = enabledSlotIdx + 1;
    };
}

bool updateMagicCommand(u8 partyIndex, u32 actorStatusMask) {
    auto actorMP = G_ACTOR_TIMER_ARRAY[partyIndex].currentMP;
    auto& spellData = getSrPartyMember(partyIndex).srPartyMember->actorMagics;

    bool commandEnabled = false;
    for (auto it = begin(spellData); it != end(spellData); ++it) {
        auto spellID = it->magicIndex;
        u8 spellFlags = 2; //start by disabling the spell
        if (spellID != 0xFF) {
            auto targetData = it->targetData;
            if ((targetData & 8) && !(it->allCount) && !(it->quadCount)) { //if toggle multiple, but no all uses
                targetData &= 0xF3u; //unset toggle multiple
            }
            if (targetData & 8) //if toggle multiple
                spellFlags = 18;
            if (it->quadCount) {
                spellFlags |= 0x10u;
                targetData = (targetData & 0xF7) | 4;
            }

            it->targetData = targetData;
            if ((actorMP >= it->mpCost) && !(actorStatusMask & 0x80) && ((actorStatusMask & 0x800) == 0 || spellID == 10)) {
                spellFlags &= 0xFDu; 
                commandEnabled = true;                       
            }
        }
        it->propertiesMask = spellFlags;
    }
    return commandEnabled;
}

/*With this we can add a charge mechanic to summons*/
bool updateSummonCommand(u8 partyIndex, u32 actorStatusMask) {
    auto actorMP = G_ACTOR_TIMER_ARRAY[partyIndex].currentMP;
    auto& summonData = getSrPartyMember(partyIndex).srPartyMember->actorSummons;

    bool commandEnabled = false;
    for (auto it = begin(summonData); it != end(summonData); ++it) {
        u8 spellFlags = 2;
        if (it->magicIndex != 0xFF) {
            if ((actorMP >= it->mpCost) && !(actorStatusMask & 0x80) && (it->allCount)) {
                spellFlags &= 0xFDu; 
                commandEnabled = true;  
            }
        }
        it->propertiesMask = spellFlags;
    }
    return commandEnabled;
}

bool updateESkillCommand(u8 partyIndex, u32 actorStatusMask) {
    auto actorMP = G_ACTOR_TIMER_ARRAY[partyIndex].currentMP;
    auto& ESkillData = getSrPartyMember(partyIndex).srPartyMember->actorEnemySkills;

    bool commandEnabled = false;
    for (auto it = begin(ESkillData); it != end(ESkillData); ++it) {
        u8 spellFlags = 2; 
        if (it->magicIndex != 0xFF) {
            if ((actorMP >= it->mpCost) && !(actorStatusMask & 0x80)) {
                spellFlags &= 0xFDu;
                commandEnabled = true;            
            }
        }
        it->propertiesMask = spellFlags;
    }
    return commandEnabled;
}

