#include "party.h"
#include "../events/party_events.h"
#include "../impl.h"

SrPartyDataRegistry::SrPartyDataRegistry() : SrNamedResourceRegistry<SrPartyData, std::string>() {
    add_element(getPartyKey(0), SrPartyData());
    add_element(getPartyKey(1), SrPartyData());
    add_element(getPartyKey(2), SrPartyData());
}

void SrPartyDataRegistry::addAutoAction(u32 partyIndex, const SrAutoAction& action) {
    auto& autoActons = get_element(getPartyKey(partyIndex)).actorAutoActions;
    for (auto& actionSlot : autoActons) {
        if (actionSlot.dispatchType == AUTOACT_NO_ACTION) {
            actionSlot = action;
        }
    }
}

/* clear all data arrays prior to recalculation based on current equipment configuration*/
void SrPartyDataRegistry::clearActions(u32 partyIndex) {
    auto& actor = get_element(getPartyKey(partyIndex));
    auto& magicArray = actor.actorMagics;
    auto& summonArray = actor.actorSummons;
    auto& eSkillArray = actor.actorEnemySkills;
    auto& autoActions = actor.actorAutoActions;
    clearActionArray<MAGIC_COUNT>(magicArray);
    clearActionArray<SUMMON_COUNT>(summonArray);
    clearActionArray<ESKILL_COUNT>(eSkillArray);
    clearAutoActionArray<AUTO_ACTION_COUNT>(autoActions);
    clearCommandArray(partyIndex);
}

const std::string getPartyKey(u8 partyIndex) {
    return std::to_string(partyIndex) + std::string("PARTY");
}

/*This method enables actions*/
void SrPartyDataRegistry::handleMateriaActorUpdates(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, ActorStatBoosts& boosts) {
    if (partyIndex > 3)
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
    enableDefaultCommands(partyIndex, magicEnabled, summonEnabled);

    for (auto it = begin(equippedMaterias); it != end(equippedMaterias); ++it) {
        auto materia = *it;
        if (materia.item_id == 0xFFFF)
            continue;
        u8 maxLevel = 1;
        auto materiaLevel = getMateriaLevel(materia, &maxLevel);
        EnableAbilitiesEvent enableActionEvent = { partyIndex, materia, gContext.materias.get_resource(materia.item_id), materiaLevel, &boosts };
        auto topkey = getTopKey(getMateriaTopType(materia.item_id));
        auto subkey = getSubKey(getMateriaSubType(materia.item_id));
        std::vector<SrEventContext> dispatchContexts = { topkey, subkey };
        gContext.eventBus.dispatch(ENABLE_ACTIONS, &enableActionEvent, dispatchContexts);
    }
    PARTY_STRUCT_ARRAY[partyIndex].commandColumns = getCommandRows(partyIndex);
}

u8 getCommandRows(u8 partyIndex) {
    auto commands = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
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
void applyLinkedMateriaModifiers(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType, ActorStatBoosts& boosts) {
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
            dispatchSupportHandlers(partyIndex, leftMateria, rightMateria, gearType, boosts);
        }
        if (getMateriaTopType(rightMateria.item_id) == 5) {
            dispatchSupportHandlers(partyIndex, rightMateria, leftMateria, gearType, boosts);
        }
    }
}

u8* getMateriaSlots(u8 partyIdx, SrGearType gearType) {
    u8 characterRecordArrayIndex = getCharacterRecordIndex(partyIdx);

    auto kernelObjectID = getEquippedGear(characterRecordArrayIndex, gearType);

    if (gearType == SR_GEAR_WEAPON) {
        auto materiaSlots = &(gContext.weapons.get_resource(kernelObjectID).materia_slots[0]);
        return materiaSlots;
    }
    else if (gearType == SR_GEAR_ARMOR) {
        auto materiaSlots = &(gContext.armors.get_resource(kernelObjectID).materia_slots[0]);
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
void dispatchSupportHandlers(u8 partyIndex, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGearType gearType, ActorStatBoosts& boosts) {
    u8 supportMax;
    u8 pairedMax;
    auto supportLevel = getMateriaLevel(supportMateria, &supportMax);
    auto pairedLevel = getMateriaLevel(pairedMateria, &pairedMax);
    ApplySupportEvent applySupportEvent = {
        partyIndex,
        supportMateria,
        supportLevel,
        gContext.materias.get_resource(supportMateria.item_id),
        pairedMateria,
        pairedLevel,
        gContext.materias.get_resource(pairedMateria.item_id),
        &boosts
    };

    auto topkey = getTopKey(getMateriaTopType(pairedMateria.item_id));
    auto subkey = getSubKey(getMateriaSubType(pairedMateria.item_id));
    std::vector<SrEventContext> dispatchContexts = { topkey, subkey };
    gContext.eventBus.dispatch(APPLY_SUPPORT, (void*)&applySupportEvent, dispatchContexts);
}

void clearCommandArray(u8 partyIndex) {
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        voidCommand(partyIndex, slotIndex);
    }
}

/*Hooking into here we can insert default commands for characters and such*/
void enableDefaultCommands(u8 partyIndex, bool magicEnabled, bool summonEnabled) {
    enableCommand(partyIndex, 0, CMD_ATTACK);
    enableCommand(partyIndex, 3, CMD_ITEM);
    if (magicEnabled) {
        enableCommand(partyIndex, 1, CMD_MAGIC);
    }
    if (summonEnabled) {
        enableCommand(partyIndex, 2, CMD_SUMMON);
    }
}


SISTERRAY_API void addAutoAction(u8 partyIndex, AutoActionType type, u8 commandIndex, u16 actionID, u8 activationChance, u8 counterCount) {
    auto& action = SrAutoAction();
    action.dispatchType = type;
    action.commandIndex = commandIndex;
    action.actionIndex = actionID;
    action.counterCount = counterCount;
    action.activationChance = activationChance;
    gContext.party.addAutoAction(partyIndex, action);
}

SISTERRAY_API void enableCommand(u8 partyIndex, u8 enabledIndex, u8 commandIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to enable an invalid command slot");
        return;
    }
    auto& commandArray = getActivePartyMember(partyIndex)->enabledCommandArray;
    commandArray[enabledIndex].commandID = commandIndex;
    commandArray[enabledIndex].cursorCommandType = getCommand(commandIndex).gameCommand.commandMenuID;
    commandArray[enabledIndex].targetingData = getCommand(commandIndex).gameCommand.targetingFlags;
    commandArray[enabledIndex].commandFlags = 0;
}

SISTERRAY_API void voidCommand(u8 partyIndex, u8 enabledIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to void an invalid command slot");
        return;
    }
    auto& command= getActivePartyMember(partyIndex)->enabledCommandArray[enabledIndex];
    command.commandID = 0xFF;
    command.cursorCommandType = 0;
    command.targetingData = 0xFF;
    command.commandFlags = 1;
    command.allCount = 0;
    command.supportMatFlags = 0;
}

SISTERRAY_API u8 getEnabledSlotIndex(u8 partyIndex, u8 commandIndex){
    auto& commandArray = getActivePartyMember(partyIndex)->enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return slotIndex;
    }
    return 0xFF;
}

/*Searches the corresponding array for now, once we have order data loaded we can just seek*/
SISTERRAY_API EnabledSpell* getSpellSlot(u8 partyIndex, u8 commandIndex, u16 actionIndex) {
    auto& partyData = gContext.party.get_element(getPartyKey(partyIndex));
    switch (commandIndex) {
        case 2: {
            for (auto& spell : partyData.actorMagics) {
                if (spell.magicIndex == actionIndex)
                    return &spell;
            }
        }
        case 3: {
            for (auto& spell : partyData.actorSummons) {
                if (spell.magicIndex == actionIndex)
                    return &spell;
            }
        }
        case 14:{
            for (auto& spell : partyData.actorEnemySkills) {
                if (spell.magicIndex == actionIndex)
                    return &spell;
            }
        }        
        default: {
            return nullptr;
        }
    }
}

/*Insert a given command index, enabling it. Will not insert at Magic/Command/Summon indexes*/
SISTERRAY_API void  insertEnabledCommand(u8 partyIndex, u8 commandIndex) {
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    u8 freeIndex = 0xFF;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == 0xFF) {
            freeIndex = slotIndex;
            break;
        }
        if (commandArray[slotIndex].commandID == commandIndex) {
            return;
        }
    }
    if (freeIndex != 0xFF)
        enableCommand(partyIndex, freeIndex, commandIndex);
}

SISTERRAY_API EnabledCommandStruct* getCommandSlot(u8 partyIndex, u8 commandIndex) {
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return &(commandArray[slotIndex]);
    }
    return nullptr;
}

/*These functions will be acessible through the C API, and so return raw pointers*/
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u8 partyIndex, u32 enabledSlotIndex) {
    auto& magicArray = gContext.party.get_element(getPartyKey(partyIndex)).actorMagics;
    if (enabledSlotIndex < magicArray.max_size()) {
        auto partyPtr = magicArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid spell index");
    return nullptr;
}

/*Public API methods for enabling an action at a specific command index*/
SISTERRAY_API void enableMagic(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledMagics = gContext.party.get_element(getPartyKey(partyIndex)).actorMagics;
    if (enabledIndex < enabledMagics.max_size()) {
        auto& enabledSlot = enabledMagics[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u8 partyIndex, u32 enabledSlotIndex) {
    auto& summonArray = gContext.party.get_element(getPartyKey(partyIndex)).actorSummons;
    if (enabledSlotIndex < summonArray.max_size()) {
        auto partyPtr = summonArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableSummon(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledSummons = gContext.party.get_element(getPartyKey(partyIndex)).actorSummons;
    if (enabledIndex < enabledSummons.max_size()) {
        auto& enabledSlot = enabledSummons[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_SUMMON, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_SUMMON, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}


SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u8 partyIndex, u32 enabledSlotIndex) {
    auto& ESkillArray = gContext.party.get_element(getPartyKey(partyIndex)).actorEnemySkills;
    if (enabledSlotIndex < ESkillArray.max_size()) {
        auto partyPtr = ESkillArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableESkill(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledESkills = gContext.party.get_element(getPartyKey(partyIndex)).actorEnemySkills;
    if (enabledIndex < enabledESkills.max_size()) {
        auto& enabledSlot = enabledESkills[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

void srUpdatePartyMember(u32 partyIndex) {
    recalculateBaseStats(partyIndex);
    recalculateDerivedStats(partyIndex); //This function will be replaced with an sr version to correctly init the new command fields
    updateMiscPartyStats();
}

/*Handler handle updating active commands*/
void srRecalculateDerivedStats(u32 partyIndex) {
    auto characterID = getCharacterRecordIndex(partyIndex);
    auto characterName = getCharacterName(characterID);
    auto weaponMaterias = gContext.characters.get_element(characterName).wpnMaterias;
    auto armorMaterias = gContext.characters.get_element(characterName).armMaterias;

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

    gContext.party.clearActions(partyIndex);
    ActorStatBoosts statBoosts = createActorStatBoosts();
    gContext.party.handleMateriaActorUpdates(partyIndex, equippedMaterias, statBoosts); //Enable spells, counters, commands, and fill out stat boosts based on materia stuff
    applyLinkedMateriaModifiers(partyIndex, wpnVector, SR_GEAR_WEAPON, statBoosts);
    applyLinkedMateriaModifiers(partyIndex, armVector, SR_GEAR_ARMOR, statBoosts);
}

/*determine whether or not commands are enabled from resizeable SR arrays*/
void updateCommandsActive(i32 partyIndex, i32 commandType) {

    if ((partyIndex < 3) && (1 << commandType) & gActorTimerBlock[partyIndex].activeCommandsMask) {
        i32 statusMask = gAiActorVariables[partyIndex].statusMask;
        gActorTimerBlock[partyIndex].activeCommandsMask &= ~(u8)(1 << commandType);
        switch (commandType) {
            case 0: {
                updateCommands(partyIndex, statusMask);
                break;
            }
            case 1: {
                gActorTimerBlock[partyIndex].activeCommandsMask &= 0xDFu;// Unset Bit 0x20
                if (!updateMagicCommand(partyIndex, statusMask)) {
                    gActorTimerBlock[partyIndex].activeCommandsMask |= 0x20;
                }
                break;
            }
            case 2: {
                gActorTimerBlock[partyIndex].activeCommandsMask &= 0xBFu;// Unset bit 0x40
                if (!updateSummonCommand(partyIndex, statusMask))
                    gActorTimerBlock[partyIndex].activeCommandsMask |= 0x40;
                break;
            }
            case 3: {
                gActorTimerBlock[partyIndex].activeCommandsMask &= 0x7Fu;// unset bit 0x80
                if (!updateESkillCommand(partyIndex, statusMask))
                    gActorTimerBlock[partyIndex].activeCommandsMask |= 0x80;
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
    auto enabledCommands = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
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
                    if (gActorTimerBlock[partyIndex].activeCommandsMask & 0x20) {
                        srLogWrite("A:Flagging command %i inactive", commandID);
                        commandFlags |= 2u;
                    }
                    break;
                }
                case 3:
                case 22: {
                    updateCommandsActive(partyIndex, 2);
                    if (gActorTimerBlock[partyIndex].activeCommandsMask & 0x40) {
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
                    if (gActorTimerBlock[partyIndex].activeCommandsMask & 0x80) {
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
    auto actorMP = gActorTimerBlock[partyIndex].currentMP;
    auto& spellData = gContext.party.get_element(getPartyKey(partyIndex)).actorMagics;

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
    auto actorMP = gActorTimerBlock[partyIndex].currentMP;
    auto& summonData = gContext.party.get_element(getPartyKey(partyIndex)).actorSummons;

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
    auto actorMP = gActorTimerBlock[partyIndex].currentMP;
    auto& ESkillData = gContext.party.get_element(getPartyKey(partyIndex)).actorEnemySkills;

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

