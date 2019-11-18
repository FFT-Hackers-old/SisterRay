#include "cmd_select_callbacks.h"
#include "../battle/engine/battle_engine_interface.h"

void handleCommandTarget(SelectCommandEvent* event) {
    // attack case
    u8* byte_DC3C8C = (u8*)0xDC3C8C;
    *byte_DC3C8C = 1;
    *PREVIOUS_BATTLE_MENU_STATE = 1;
    setMenuState(event->menuObect, BATTLE_TARGETING_STATE);
    setTargetingFromFlags(event->command->targetingData, false);
}

void handleWeaponTarget(SelectCommandEvent* event) {
    // attack case
    u8* byte_DC3C8C = (u8*)0xDC3C8C;
    *byte_DC3C8C = 0;
    *PREVIOUS_BATTLE_MENU_STATE = 1;
    setMenuState(event->menuObect, BATTLE_TARGETING_STATE);
    setTargetingFromFlags(event->command->targetingData, false);
}

void cmdMagicSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_MAGIC_STATE);
}

void cmdSummonSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_SUMMON_STATE);
}

void cmdItemSelectHandler(SelectCommandEvent* event) {
    setMenuState(event->menuObect, BATTLE_ITEM_STATE);
}

/*void chooseBattleMenuCommands()
{
    __int16 relativeRowIdx; // ST10_2
    __int16 relativeColumnIdx; // [esp+8h] [ebp-18h]
    signed int i; // [esp+Ch] [ebp-14h]
    __int16 v3; // [esp+10h] [ebp-10h]
    __int16 v4; // [esp+14h] [ebp-Ch]
    EnabledCommand *enabledCommandsPtr; // [esp+18h] [ebp-8h]
    int *cursorContextPtr; // [esp+1Ch] [ebp-4h]

    updateCommandsActive((unsigned __int8)ACTION_ISSUING_ACTOR, 0);
    if (!byte_DC35A8)
    {
        playMenuSound(2);
        byte_DC35A8 = 1;
    }
    cursorContextPtr = &dword_DC20A0[448 * (unsigned __int8)ACTION_ISSUING_ACTOR];
    enabledCommandsPtr = PartyDataArray[(unsigned __int8)ACTION_ISSUING_ACTOR].enabledCommandArray;

    if (BATTLE_MENU_STATE == 1 && !ACCEPTING_BATTLE_INPUT) {
        relativeColumnIdx = *(u16*)cursorContextPtr;
        relativeRowIdx = *((_WORD *)cursorContextPtr + 2);
        handleCursorPositionUpdate(cursorContextPtr);
        v4 = *cursorContextPtr - relativeColumnIdx;
        v3 = cursorContextPtr[1] - relativeRowIdx;
        for (i = 0;
            i < 20 && (unsigned __int8)enabledCommandsPtr[cursorContextPtr[1] + 4 * *cursorContextPtr].commandID == 255;
            ++i)
        {
            cursorContextPtr[1] += v3;
            *cursorContextPtr += v4;
            cursorContextPtr[1] &= 3u;
            *cursorContextPtr %= (signed int)(unsigned __int8)PartyDataArray[(unsigned __int8)ACTION_ISSUING_ACTOR].commandColumns;
        }
        if (*cursorContextPtr == relativeColumnIdx && receivedInput2(0x8000))
        {
            playMenuSound(1);
            ISSUED_COMMAND_ID = 18;
            BATTLE_MENU_STATE = 2;
            if (dword_DC130C == 1)
                initHandlerCursorState(
                    *((_WORD *)PtrToBattleMenuArrays + 76) - 48,
                    12 * *((_WORD *)cursorContextPtr + 2) + 169,
                    2);
            else
                initHandlerCursorState(
                    *((_WORD *)PtrToBattleMenuArrays + 76) - 96,
                    24 * *((_WORD *)cursorContextPtr + 2) + 338,
                    2);
            ACCEPTING_BATTLE_INPUT = 1;
        }
        else if (*cursorContextPtr == relativeColumnIdx && receivedInput2(0x2000))
        {
            playMenuSound(1);
            ISSUED_COMMAND_ID = 19;
            BATTLE_MENU_STATE = 3;
            if (dword_DC130C == 1)
                initHandlerCursorState(
                    *((_WORD *)PtrToBattleMenuArrays + 76) + *((_WORD *)PtrToBattleMenuArrays + 78) - 8,
                    12 * *((_WORD *)cursorContextPtr + 2) + 169,
                    3);
            else
                initHandlerCursorState(
                    *((_WORD *)PtrToBattleMenuArrays + 76) + *((_WORD *)PtrToBattleMenuArrays + 78) - 16,
                    24 * *((_WORD *)cursorContextPtr + 2) + 338,
                    3);
            ACCEPTING_BATTLE_INPUT = 1;
        }
        else if (receivedInput2(32))              // handle OK input on a command
        {
            byte_DC3C8C = 1;
            ACCEPTING_BATTLE_INPUT = 1;
            if (enabledCommandsPtr[4 * *cursorContextPtr + cursorContextPtr[1]].commandFlags & 2)// checks if the command is enabled
            {
                playMenuSound(3);
            }
            else
            {
                playMenuSound(1);
                COMMAND_MENU_INDEX = enabledCommandsPtr[4 * *cursorContextPtr + cursorContextPtr[1]].cursorActionID;
                ISSUED_COMMAND_ID = enabledCommandsPtr[cursorContextPtr[1] + 4 * *cursorContextPtr].commandID;
                VALID_TARGET_MASK = word_9A889A;
                W_COMMAND_ENABLED = 0;
                switch (COMMAND_MENU_INDEX)
                {
                    case 0:                               // attack case
                        byte_DC3C8C = 1;
                        PREVIOUS_BATTLE_MENU_STATE = 1;
                        BATTLE_MENU_STATE = 0;
                        GLOBAL_USED_ACTION_TARGET_DATA = (unsigned __int8)enabledCommandsPtr[4 * *cursorContextPtr
                            + cursorContextPtr[1]].targetingData;
                        setCursorTargetingData();
                        return;
                    case 1:                               // magic_case
                        goto LABEL_27;
                    case 2:
                        goto LABEL_31;
                    case 3:
                        goto LABEL_29;
                    case 4:
                        BATTLE_MENU_STATE = 4;
                        ACCEPTING_BATTLE_INPUT = 1;
                        initHandlerCursorState(-1, -1, 4);
                        break;
                    case 5:
                        BATTLE_MENU_STATE = 5;
                        initHandlerCursorState(-1, -1, 5);
                        break;
                    case 6:                               // limit
                        byte_DC3C80 = ACTION_ISSUING_ACTOR;
                        ACCEPTING_BATTLE_INPUT = 1;
                        if (partyIndexCharacterID[(unsigned __int8)ACTION_ISSUING_ACTOR] == 2)// Tifa's slots
                        {
                            BATTLE_MENU_STATE = 27;
                            initHandlerCursorState(-1, -1, 27);
                            sub_430CFA(0x1D3u);
                        }
                        else if (partyIndexCharacterID[(unsigned __int8)ACTION_ISSUING_ACTOR] == 6)
                        {
                            if (unk_DC00B2 == 2)
                            {
                                BATTLE_MENU_STATE = 26;         // slots
                                initHandlerCursorState(-1, -1, 26);
                                sub_430CFA(0x1D3u);
                            }
                            else
                            {
                                BATTLE_MENU_STATE = 24;
                                initHandlerCursorState(-1, -1, 24);
                            }
                        }
                        else
                        {
                            BATTLE_MENU_STATE = 24;
                            initHandlerCursorState(-1, -1, 24);
                        }
                        break;
                    case 7:
                        byte_DC3C8C = 0;
                        PREVIOUS_BATTLE_MENU_STATE = 1;
                        BATTLE_MENU_STATE = 0;
                        GLOBAL_USED_ACTION_TARGET_DATA = (unsigned __int8)enabledCommandsPtr[4 * *cursorContextPtr
                            + cursorContextPtr[1]].targetingData;
                        setCursorTargetingData();
                        break;
                    case 8:
                        W_COMMAND_ENABLED = 1;
                    LABEL_27:
                        BATTLE_MENU_STATE = 6;
                        initHandlerCursorState(-1, -1, 6);
                        break;
                    case 9:
                        W_COMMAND_ENABLED = 1;
                    LABEL_31:
                        BATTLE_MENU_STATE = 7;
                        ACCEPTING_BATTLE_INPUT = 1;
                        initHandlerCursorState(-1, -1, 7);
                        break;
                    case 0xA:
                        W_COMMAND_ENABLED = 1;
                    LABEL_29:
                        BATTLE_MENU_STATE = 5;
                        initHandlerCursorState(-1, -1, 5);
                        break;
                    case 0xB:
                        if (dword_DC1F44 & 0x40)
                        {
                            BATTLE_MENU_STATE = 20;
                            ACCEPTING_BATTLE_INPUT = 1;
                            initHandlerCursorState(-1, -1, 20);
                        }
                        else
                        {
                            BATTLE_MENU_STATE = 5;
                            ACCEPTING_BATTLE_INPUT = 1;
                            COMMAND_MENU_INDEX = 5;
                            ISSUED_COMMAND_ID = 8;
                            initHandlerCursorState(-1, -1, 5);
                        }
                        break;
                    default:
                        return;
                }
            }
        }
    }
}*/
