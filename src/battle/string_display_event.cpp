#include "string_display_event.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"
#include "scene_globals.h"
#include "../impl.h"

using namespace BattleMenuWidgetNames;

void nopBattleString(u32 a1) {
    Menu* menu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    MenuDrawEvent srEvent = { menu, getMenuState(menu), 0 };
    handleTopDisplayString(&srEvent);
    return;
}

void handleTopDisplayString(const MenuDrawEvent* event) {
    u8* battleTextActive = (u8*)0xDC38BC;
    u16* stringBufferIdx = (u16*)0xDC38C0;
    u32* stringCommandIdx = (u32*)0xDC38EC;
    u32* stringActionIdx = (u32*)0xDC38F0;
    u32* actionDisplayActive = (u32*)0xDC38E8;
    u32* dword_90C9F0 = (u32*)0x90C9F0;
    char argSubBuffer[512] = {};

    if (!(*dword_90C9F0)) {
        return;
    }

    auto topDisplayWidget = getChild(getChild(event->menu->menuWidget, BATTLE_BASE_WIDGET_NAME), TOP_STRING_DISPLAY);
    if (*battleTextActive) {
        enableWidget(topDisplayWidget);
        if (*stringBufferIdx < 256) {
            auto baseStr = gContext.gameStrings.battle_texts.get_string(*stringBufferIdx);
            handleBattleStrSubstitions(&(argSubBuffer[0]), baseStr);
            updateText(getChild(topDisplayWidget, "TXT"), &(argSubBuffer[0]));
            BoxWidget* box = (BoxWidget*)getChild(topDisplayWidget, "BOX");
            auto stringWidth = 12 * (EncodedString(&(argSubBuffer[0])).size());
            auto x = (box->drawDistanceXa + (box->drawDistanceYa / 2)) - (stringWidth / 2);
            auto y = box->drawDistanceXb + (box->drawDistanceYb / 4);
            moveWidget(getChild(topDisplayWidget, "TXT"), x, y);
            return;
        }
        else {
            u16 gStringBufferIdx = *stringBufferIdx - 256;
            auto baseStr = srGetStrFromGlobalBuffer(gStringBufferIdx);
            handleBattleStrSubstitions(&(argSubBuffer[0]), baseStr);
            updateText(getChild(topDisplayWidget, "TXT"), &(argSubBuffer[0]));
            BoxWidget* box = (BoxWidget*)getChild(topDisplayWidget, "BOX");
            auto stringWidth = 12 * (EncodedString(&(argSubBuffer[0])).size());
            auto x = (box->drawDistanceXa + (box->drawDistanceYa / 2)) - (stringWidth / 2);
            auto y = box->drawDistanceXb + (box->drawDistanceYb / 4);
            moveWidget(getChild(topDisplayWidget, "TXT"), x, y);
            return;
        }
    }
    else if (*actionDisplayActive) {
        enableWidget(topDisplayWidget);
        auto textToDraw = getCommandAction(*stringCommandIdx, *stringActionIdx).attackName.str();
        updateText(getChild(topDisplayWidget, "TXT"), textToDraw); // Will need some code to center the text+
        BoxWidget* box = (BoxWidget*)getChild(topDisplayWidget, "BOX");
        auto stringWidth = 12 * (EncodedString(textToDraw).size());
        auto x = (box->drawDistanceXa + (box->drawDistanceYa / 2)) - (stringWidth / 2);
        auto y = box->drawDistanceXb + (box->drawDistanceYb / 4);
        moveWidget(getChild(topDisplayWidget, "TXT"), x, y);
        *actionDisplayActive = 0;
        return;
    }
    disableWidget(topDisplayWidget);
}

char* srGetStrFromGlobalBuffer(u16 strBufferIndex) {
    char* G_DISPLAY_STR_BUFFER = (char*)0x9AD1E0;
    u16 * G_STRBUFFER_ID_OFFSET_MAP = (u16*)0x9AD9E0;
    return &(G_DISPLAY_STR_BUFFER[G_STRBUFFER_ID_OFFSET_MAP[strBufferIndex]]);
}

void handleBattleStrSubstitions(char* ret, const char* base) {
    u16 baseStrIdx = 0;
    u16 retStrIdx = 0;
    u16 wordArg = 0;
    char currentChar = base[baseStrIdx];
    const char* resourceName;
    const u16* wordReader = (const u16*)base;
    srLogWrite("Handling variable string substition for string %s", base);
    while (currentChar != char(0xFF)) {
        switch (currentChar) {
        case 0xEB: {
            wordArg = wordReader[baseStrIdx + 1];
            resourceName = getItemNameFromAbsoluteIdx(wordArg);
            retStrIdx += insertEncodedStr(ret, retStrIdx, resourceName);
            baseStrIdx += 3;
            break;
        case 0xEC: {
            wordArg = wordReader[baseStrIdx + 1];
            auto number = std::to_string(wordArg);
            EncodedString numberStr = EncodedString(number.c_str());
            retStrIdx += insertEncodedStr(ret, retStrIdx, numberStr.str());
            baseStrIdx += 3;
            break;
        }
        case 0xED: {
            wordArg = wordReader[baseStrIdx + 1]; //Holds the actor, and displays target Name
            if (wordArg > 4) {
                resourceName = &(getRegistryActorEnemyData(wordArg - 4).enemyData.enemyName[0]);
                retStrIdx += insertEncodedStr(ret, retStrIdx, resourceName);
            }
            else {
                resourceName = &(getPartyActorCharacterRecord(wordArg)->character_name[0]);
                retStrIdx += insertEncodedStr(ret, retStrIdx, resourceName);
            }
            baseStrIdx += 3;
            break;
        }
        case 0xEE: {
            // Get the name of the attack based on an absolute ID. Will need to modify calling code
            wordArg = wordReader[baseStrIdx + 1];
            resourceName = gContext.attacks.getResource(wordArg).attackName.str();
            retStrIdx += insertEncodedStr(ret, retStrIdx, resourceName);
            baseStrIdx += 3;
            break;
        }
        case 0xEF: {
            wordArg = wordReader[baseStrIdx + 1];
            char letter[2] = { char(wordArg + 0x21), char(0xFF) };
            retStrIdx += insertEncodedStr(ret, retStrIdx, &(letter[0]));
            baseStrIdx += 3;
            break;
        }
        case 0xF0: {
            wordArg = wordReader[baseStrIdx + 1];
            resourceName = gContext.gameStrings.elementNames.get_string(wordArg);
            retStrIdx += insertEncodedStr(ret, retStrIdx, resourceName);
            baseStrIdx += 3;
            break;
        }
        case 0xF8: {
            baseStrIdx += 2;
            break;
        }
        }
        default: {
            ret[retStrIdx] = base[baseStrIdx];
            retStrIdx++;
            break;
        }
        }
        baseStrIdx++;
        currentChar = base[baseStrIdx];
        srLogWrite("base: %s, ret:%s, base str idx: %d, current character:%d", EncodedString(base).unicode(), EncodedString(ret).unicode(), baseStrIdx, (u8)currentChar);
    }
    ret[retStrIdx] = char(0xFF);
}

// Does not copy the terminating 0xFF character, returns bytes copied
u16 insertEncodedStr(char* dst, u16 baseStart, const char* toInject) {
    u16 injectIdx = 0;
    u16 bytesCopied = 0;
    while (toInject[injectIdx] != 0xFF) {
        dst[baseStart] = toInject[injectIdx];
        injectIdx++;
        bytesCopied++;
    }
    return bytesCopied;
}
