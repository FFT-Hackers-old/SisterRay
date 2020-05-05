#include "party_actions.h"
#include "party.h"
#include "party_utils.h"
#include "../impl.h"

void clearCommandArray(u8 characterIdx) {
    auto& commandArray = *getSrCharacter(characterIdx).gamePartyMember->enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        voidCommand(characterIdx, slotIndex);
    }
}

void clearSummonCommandArray(u8 summonIdx) {
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        auto& command = gContext.party.getSrSummon(summonIdx).gamePartyMember->enabledCommandArray[slotIndex];
        voidCommand(command, slotIndex);
    }
}

void voidCommand(EnabledCommand& command, u8 enabledIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to void an invalid command slot");
        return;
    }
    command.commandID = 0xFF;
    command.cursorCommandType = 0;
    command.targetingData = 0xFF;
    command.commandFlags = 1;
    command.allCount = 0;
    command.supportMatFlags = 0;
}

/*Hooking into here we can insert default commands for characters and such*/
void enableDefaultCommands(u8 characterIdx, bool magicEnabled, bool summonEnabled) {
    enableCommand(characterIdx, 0, CMD_ATTACK);
    enableCommand(characterIdx, 3, CMD_ITEM);
    if (magicEnabled) {
        enableCommand(characterIdx, 1, CMD_MAGIC);
    }
    if (summonEnabled) {
        enableCommand(characterIdx, 2, CMD_SUMMON);
    }
}


SISTERRAY_API void addAutoAction(u8 characterIdx, AutoActionType type, u8 commandIndex, u16 actionID, u8 activationChance, u8 counterCount) {
    auto& action = SrAutoAction();
    action.dispatchType = type;
    action.commandIndex = commandIndex;
    action.actionIndex = actionID;
    action.counterCount = counterCount;
    action.activationChance = activationChance;
    gContext.party.addAutoAction(characterIdx, action);
}


SISTERRAY_API void enableActorCommand(u8 actorIdx, u8 enabledIndex, const char* modName, u8 modCmdIdx) {

    if (enabledIndex >= 16) {
        srLogWrite("attempt to enable an invalid command slot");
        return;
    }
    auto name = std::string(modName) + std::to_string(modCmdIdx);
    auto& commandArray = gContext.party.getActivePartyMember(actorIdx).gamePartyMember->enabledCommandArray;
    commandArray[enabledIndex].commandID = gContext.commands.getResourceIndex(name);
    commandArray[enabledIndex].cursorCommandType = getCommand(commandArray[enabledIndex].commandID).gameCommand.commandMenuID;
    commandArray[enabledIndex].targetingData = getCommand(commandArray[enabledIndex].commandID).gameCommand.targetingFlags;
    commandArray[enabledIndex].commandFlags = 0;
    srLogWrite("UPDATED COMMAND AT %i to IDX: %i", enabledIndex, commandArray[enabledIndex].commandID);
}

SISTERRAY_API void setToggleCommand(u8 partyIdx, u8 enabledIndex, const char* modName, u8 modCmdIdx) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to enable an invalid command slot");
        return;
    }
    auto name = std::string(modName) + std::to_string(modCmdIdx);
    auto& commandArray = gContext.party.getActivePartyMember(partyIdx).gamePartyMember->enabledCommandArray;
    auto& toggleCommand =  gContext.party.getActivePartyMember(partyIdx).srPartyMember->toggleCommandArray[enabledIndex];
    toggleCommand.commandID = commandArray[enabledIndex].commandID;
    toggleCommand.cursorCommandType = commandArray[enabledIndex].cursorCommandType;
    toggleCommand.targetingData = commandArray[enabledIndex].targetingData;
    toggleCommand.commandFlags = 0;
    enableActorCommand(partyIdx, enabledIndex, modName, modCmdIdx);
}

SISTERRAY_API void toggleBack(u8 partyIdx, u8 enabledIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to enable an invalid command slot");
        return;
    }
    auto& activeCommand = gContext.party.getActivePartyMember(partyIdx).gamePartyMember->enabledCommandArray[enabledIndex];
    auto& toggleCommand = gContext.party.getActivePartyMember(partyIdx).srPartyMember->toggleCommandArray[enabledIndex];
    auto swap = activeCommand;
    activeCommand = toggleCommand;
    toggleCommand = swap;
    srLogWrite("Toggling Command at Idx: %i back to cmd: %i", enabledIndex, activeCommand.commandID);
}

SISTERRAY_API void enableCommand(u8 characterIdx, u8 enabledIndex, u8 commandIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to enable an invalid command slot");
        return;
    }
    auto commandArray = gContext.party.getSrCharacter(characterIdx).gamePartyMember->enabledCommandArray;
    commandArray[enabledIndex].commandID = commandIndex;
    commandArray[enabledIndex].cursorCommandType = getCommand(commandIndex).gameCommand.commandMenuID;
    commandArray[enabledIndex].targetingData = getCommand(commandIndex).gameCommand.targetingFlags;
    commandArray[enabledIndex].commandFlags = 0;
}


SISTERRAY_API void enableSummonCommand(u8 summonIdx, u8 enabledIndex, u8 commandIndex) {
    auto commandArray = gContext.party.getSrSummon(summonIdx).gamePartyMember->enabledCommandArray;
    commandArray[enabledIndex].commandID = commandIndex;
    commandArray[enabledIndex].cursorCommandType = getCommand(commandIndex).gameCommand.commandMenuID;
    commandArray[enabledIndex].targetingData = getCommand(commandIndex).gameCommand.targetingFlags;
    commandArray[enabledIndex].commandFlags = 0;
}

SISTERRAY_API void voidCommand(u8 characterID, u8 enabledIndex) {
    if (enabledIndex >= 16) {
        srLogWrite("attempt to void an invalid command slot");
        return;
    }
    auto& command = gContext.party.getSrCharacter(characterID).gamePartyMember->enabledCommandArray[enabledIndex];
    command.commandID = 0xFF;
    command.cursorCommandType = 0;
    command.targetingData = 0xFF;
    command.commandFlags = 1;
    command.allCount = 0;
    command.supportMatFlags = 0;
}

SISTERRAY_API u8 getEnabledSlotIndex(u8 characterIdx, u8 commandIndex) {
    auto& commandArray = gContext.party.getSrCharacter(characterIdx).gamePartyMember->enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return slotIndex;
    }
    return 0xFF;
}

/*Searches the corresponding array for now, once we have order data loaded we can just seek*/
SISTERRAY_API EnabledSpell* getSpellSlot(u8 characterIdx, u8 commandIndex, u16 actionIndex) {
    auto& partyData = getSrPartyMember(characterIdx);
    switch (commandIndex) {
    case 2: {
        for (auto& spell : partyData.srPartyMember->actorMagics) {
            if (spell.magicIndex == actionIndex)
                return &spell;
        }
    }
    case 3: {
        for (auto& spell : partyData.srPartyMember->actorSummons) {
            if (spell.magicIndex == actionIndex)
                return &spell;
        }
    }
    case 14: {
        for (auto& spell : partyData.srPartyMember->actorEnemySkills) {
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
SISTERRAY_API void  insertEnabledCommand(u8 characterIdx, u8 commandIndex) {
    auto& commandArray = gContext.party.getSrCharacter(characterIdx).gamePartyMember->enabledCommandArray;
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
        enableCommand(characterIdx, freeIndex, commandIndex);
}

SISTERRAY_API EnabledCommand* getCommandSlot(u8 characterIdx, u8 commandIndex) {
    auto& commandArray = getSrCharacter(characterIdx).gamePartyMember->enabledCommandArray;
    for (auto slotIndex = 0; slotIndex < 16; slotIndex++) {
        if (commandArray[slotIndex].commandID == commandIndex)
            return &(commandArray[slotIndex]);
    }
    return nullptr;
}

/*These functions will be acessible through the C API, and so return raw pointers*/
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u8 characterIdx, u32 enabledSlotIndex) {
    auto& magicArray = getSrCharacter(characterIdx).srPartyMember->actorMagics;
    if (enabledSlotIndex < magicArray.max_size()) {
        auto partyPtr = magicArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid spell index");
    return nullptr;
}

/*Public API methods for enabling an action at a specific command index*/
SISTERRAY_API void enableMagic(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex) {
    srLogWrite("enabling magic %d and index %d", commandlRelativeIndex, enabledIndex);
    auto& enabledMagics = getSrCharacter(characterIdx).srPartyMember->actorMagics;
    if (enabledIndex < enabledMagics.max_size()) {
        auto& enabledSlot = enabledMagics[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.MPCost & 0xFF; //Narrowing conversion due to game structs
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

SISTERRAY_API void enableSummonMagic(u8 summonIdx, u32 enabledIndex, u32 commandlRelativeIndex) {
    srLogWrite("enabling magic %d and index %d", commandlRelativeIndex, enabledIndex);
    auto& enabledMagics = gContext.party.getSrSummon(summonIdx).srPartyMember->actorMagics;
    if (enabledIndex < enabledMagics.max_size()) {
        auto& enabledSlot = enabledMagics[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_MAGIC, commandlRelativeIndex).attackData.MPCost & 0xFF; //Narrowing conversion due to game structs
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}

SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u8 characterIdx, u32 enabledSlotIndex) {
    auto& summonArray = getSrCharacter(characterIdx).srPartyMember->actorSummons;
    if (enabledSlotIndex < summonArray.max_size()) {
        auto partyPtr = summonArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableSummon(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledSummons = getSrCharacter(characterIdx).srPartyMember->actorSummons;
    if (enabledIndex < enabledSummons.max_size()) {
        auto& enabledSlot = enabledSummons[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_SUMMON, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_SUMMON, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}


SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u8 characterIdx, u32 enabledSlotIndex) {
    auto& ESkillArray = getSrCharacter(characterIdx).srPartyMember->actorEnemySkills;
    if (enabledSlotIndex < ESkillArray.max_size()) {
        auto partyPtr = ESkillArray.data();
        return &(partyPtr[enabledSlotIndex]);
    }
    srLogWrite("attempting to get ptr to invalid summon index");
    return nullptr;
}


SISTERRAY_API void enableESkill(u8 characterIdx, u32 enabledIndex, u32 commandlRelativeIndex) {
    auto& enabledESkills = getSrCharacter(characterIdx).srPartyMember->actorEnemySkills;
    if (enabledIndex < enabledESkills.max_size()) {
        auto& enabledSlot = enabledESkills[enabledIndex];
        enabledSlot.magicIndex = commandlRelativeIndex;
        enabledSlot.targetData = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.targetingFlags;
        enabledSlot.mpCost = getCommandAction(CMD_ENEMY_SKILL, commandlRelativeIndex).attackData.MPCost;
        return;
    }
    srLogWrite("attempted to enable magic spell at an invalid index");
}
