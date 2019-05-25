#include "party.h"
#include "../events/party_events.h"
#include "../impl.h"

SrPartyDataRegistry::SrPartyDataRegistry() {
    add_element(getPartyKey(1), SrPartyData());
    add_element(getPartyKey(2), SrPartyData());
    add_element(getPartyKey(3), SrPartyData());
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
void SrPartyDataRegistry::handleMateriaActorUpdates(u32 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, ActorStatBoosts& boosts) {
    bool magicEnabled = false;
    bool summonEnabled = false;
    for (auto materia : equippedMaterias) {
        if (getMateriaTopType(materia.item_id) == 0x9 || getMateriaTopType(materia.item_id) == 0xA)
            magicEnabled = true;
        if (getMateriaTopType(materia.item_id) == 0xB || getMateriaTopType(materia.item_id) == 0xC)
            summonEnabled = true;
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
}

/*Applys modifiers when support materia are paired with others*/
void applyLinkedMateriaModifiers(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType, ActorStatBoosts& boosts) {
    for (auto pairIndex = 0; pairIndex < 8; pairIndex += 2) {
        auto& leftMateria = equippedMaterias[pairIndex];
        auto& rightMateria = equippedMaterias[pairIndex + 1];
        /*If either of the linked slots is a support materia, dispatch handlers*/
        if (getMateriaTopType(leftMateria.item_id) == 5) {
            dispatchSupportHandlers(partyIndex, leftMateria, rightMateria, gearType, boosts);
        }
        if (getMateriaTopType(rightMateria.item_id) == 5) {
            dispatchSupportHandlers(partyIndex, rightMateria, leftMateria, gearType, boosts);
        }
    }
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
    if (magicEnabled)
        enableCommand(partyIndex, 1, CMD_MAGIC);
    if (summonEnabled)
        enableCommand(partyIndex, 2, CMD_SUMMON);
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
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    commandArray[enabledIndex].commandID = commandIndex;
    commandArray[enabledIndex].cursorCommandType = gContext.commands.get_resource(commandIndex).commandMenuID;
    commandArray[enabledIndex].targetingData = gContext.commands.get_resource(commandIndex).targetingFlags;
    commandArray[enabledIndex].commandFlags = 0;
}

SISTERRAY_API void voidCommand(u8 partyIndex, u8 enabledIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to void an invalid command slot");
        return;
    }
    auto& command= PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray[enabledIndex];
    command.commandID = 0xFF;
    command.cursorCommandType = 0;
    command.targetingData = 0xFF;
    command.commandFlags = 1;
    command.allCount = 0;
    command.HPMPAbsorbFags = 0;
}

SISTERRAY_API u8 getEnabledSlotIndex(u8 partyIndex, u8 commandIndex){
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
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
        default: {
            return nullptr;
        }
    }
}

/*Insert a given command index, enabling it. Will not insert at Magic/Command/Summon indexes*/
SISTERRAY_API void  insertEnabledCommand(u8 partyIndex, u8 commandIndex) {
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    u8 freeIndex = 0xFF;
    for (auto slotIndex = 4; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == 0xFF)
            freeIndex = slotIndex;
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
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u32 partyIndex, u32 enabledSlotIndex) {
    auto& magicArray = gContext.party.get_element(getPartyKey(partyIndex)).actorMagics;
    if (enabledSlotIndex < magicArray.max_size()) {
        auto partyPtr = magicArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid spell index");
    return nullptr;
}

/*Public API methods for enabling an action at a specific command index*/
SISTERRAY_API void enableMagic(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledMagics = gContext.party.get_element(getPartyKey(partyIndex)).actorMagics;
    if (enabledIndex < enabledMagics.max_size()) {
        auto& enabledSlot = enabledMagics[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u32 partyIndex, u32 enabledSlotIndex) {
    auto& summonArray = gContext.party.get_element(getPartyKey(partyIndex)).actorSummons;
    if (enabledSlotIndex < summonArray.max_size()) {
        auto partyPtr = summonArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableSummon(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledSummons = gContext.party.get_element(getPartyKey(partyIndex)).actorSummons;
    if (enabledIndex < enabledSummons.max_size()) {
        auto& enabledSlot = enabledSummons[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}


SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u32 partyIndex, u32 enabledSlotIndex) {
    auto& ESkillArray = gContext.party.get_element(getPartyKey(partyIndex)).actorEnemySkills;
    if (enabledSlotIndex < ESkillArray.max_size()) {
        auto partyPtr = ESkillArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableESkill(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledESkills = gContext.party.get_element(getPartyKey(partyIndex)).actorEnemySkills;
    if (enabledIndex < enabledESkills.max_size()) {
        auto& enabledSlot = enabledESkills[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
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
    ActorStatBoosts statBoosts = ActorStatBoosts();
    gContext.party.handleMateriaActorUpdates(partyIndex, equippedMaterias, statBoosts); //Enable spells, counters, commands, and fill out stat boosts based on materia stuff
    applyLinkedMateriaModifiers(partyIndex, wpnVector, SR_GEAR_WEAPON, statBoosts);
    applyLinkedMateriaModifiers(partyIndex, armVector, SR_GEAR_ARMOR, statBoosts);
}
