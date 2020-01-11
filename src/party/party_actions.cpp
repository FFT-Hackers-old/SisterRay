#include "party_actions.h"
#include "party.h"

void clearCommandArray(u8 partyIndex) {
    auto& commandArray = *getActivePartyMember(partyIndex)->enabledCommandArray;
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
    auto& command = getActivePartyMember(partyIndex)->enabledCommandArray[enabledIndex];
    command.commandID = 0xFF;
    command.cursorCommandType = 0;
    command.targetingData = 0xFF;
    command.commandFlags = 1;
    command.allCount = 0;
    command.supportMatFlags = 0;
}

SISTERRAY_API u8 getEnabledSlotIndex(u8 partyIndex, u8 commandIndex) {
    auto& commandArray = getActivePartyMember(partyIndex)->enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return slotIndex;
    }
    return 0xFF;
}

/*Searches the corresponding array for now, once we have order data loaded we can just seek*/
SISTERRAY_API EnabledSpell* getSpellSlot(u8 partyIndex, u8 commandIndex, u16 actionIndex) {
    auto& partyData = getSrPartyMember(partyIndex);
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
    case 14: {
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

SISTERRAY_API EnabledCommand* getCommandSlot(u8 partyIndex, u8 commandIndex) {
    auto& commandArray = PARTY_STRUCT_ARRAY[partyIndex].enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return &(commandArray[slotIndex]);
    }
    return nullptr;
}

/*These functions will be acessible through the C API, and so return raw pointers*/
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u8 partyIndex, u32 enabledSlotIndex) {
    auto& magicArray = getSrPartyMember(partyIndex).srPartyMember->actorMagics;
    if (enabledSlotIndex < magicArray.max_size()) {
        auto partyPtr = magicArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid spell index");
    return nullptr;
}

/*Public API methods for enabling an action at a specific command index*/
SISTERRAY_API void enableMagic(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    srLogWrite("enabling magic %d and index %d", commandlRelativeIndex, enabledIndex);
    auto& enabledMagics = getSrPartyMember(partyIndex).srPartyMember->actorMagics;
    if (enabledIndex < enabledMagics.max_size()) {
        auto& enabledSlot = enabledMagics[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.MPCost & 0xFF; //Narrowing conversion due to game structs
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u8 partyIndex, u32 enabledSlotIndex) {
    auto& summonArray = getSrPartyMember(partyIndex).srPartyMember->actorSummons;
    if (enabledSlotIndex < summonArray.max_size()) {
        auto partyPtr = summonArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableSummon(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledSummons = getSrPartyMember(partyIndex).srPartyMember->actorSummons;
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
    auto& ESkillArray = getSrPartyMember(partyIndex).srPartyMember->actorEnemySkills;
    if (enabledSlotIndex < ESkillArray.max_size()) {
        auto partyPtr = ESkillArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableESkill(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledESkills = getSrPartyMember(partyIndex).srPartyMember->actorEnemySkills;
    if (enabledIndex < enabledESkills.max_size()) {
        auto& enabledSlot = enabledESkills[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}
