#include "action_enabled_callbacks.h"
#include "../impl.h"

void enableAblT0S2(const EnableAbilitiesEvent* const event) {
    /*this tells you the stat a particular materia type will be increasing
      Since multiple stat boosting materias are possible, We will simply calculate total modifiers and apply them at the end*/
    auto statIdx = event->materiaData.data[0];
    if (statIdx == 83) {
        //enableCounter() //Enable a physical counter attack action in the list of "counter actions"
    }
}

/*This enables all materia on the command Materia, instead of just the "highest level" command*/
void enableAblT6S1(const EnableAbilitiesEvent* const event) {
    for (auto dataIdx = 0; dataIdx < 5; dataIdx++) {
        auto commandID = event->materiaData.data[dataIdx];
        if (commandID == 0xFF) {
            return;
        }
        if (dataIdx < event->materiaLevel) //Insert a command as long as there is enough space and it is not already enabled
            insertEnabledCommand(event->partyIndex, commandID);
    }
}

/*Override the attack command with the given command Index*/
void enableAblT2S1(const EnableAbilitiesEvent* const event) {
    for (auto dataIdx = 0; dataIdx < 5; dataIdx++) {
        auto commandID = event->materiaData.data[dataIdx];
        if (commandID == 0xFF) {
            return;
        }
        if (dataIdx < event->materiaLevel) //override the attack command 
            enableCommand(event->partyIndex, 0, commandID);
    }
}

/*Handlers for the type which enables the W commands*/
void enableAblT3S3(const EnableAbilitiesEvent* const event) {
    auto commandID = event->materiaData.data[0];
    u8 replacedIndex;
    if (commandID == 0xFF) {
        return;
    }
    switch (commandID) {
        case 21:
            replacedIndex = 1;
            break;
        case 22:
            replacedIndex = 2;
            break;
        case 23:
            replacedIndex = 3;
            break;
        default:{
            replacedIndex = 0xFF;
            break;
        }
    }
    if (commandID != 0xFF)
        enableCommand(event->partyIndex, replacedIndex, commandID);
}

/*Enabler handlers for each type of materia*/
void enableAblT9S1(const EnableAbilitiesEvent* const event) {
    for (auto dataIdx = 0; dataIdx < 5; dataIdx++) {
        auto spellIdx = event->materiaData.data[dataIdx];
        if (spellIdx == 0xFF) {
            continue;
        }
        if (dataIdx < event->materiaLevel)
            enableMagic(event->partyIndex, spellIdx, spellIdx);
    }
}

void enableAblT11S3(const EnableAbilitiesEvent* const event) {
    auto spellIdx = event->materiaData.data[0];
    enableSummon(event->partyIndex, spellIdx, spellIdx);
    auto spellSlotPtr = getEnabledSummonSlot(event->partyIndex, spellIdx);
    for (auto dataIdx = 1; dataIdx < 6; dataIdx++) {
        if (dataIdx < event->materiaLevel)
            spellSlotPtr->allCount = event->materiaData.data[dataIdx];
    }
}
