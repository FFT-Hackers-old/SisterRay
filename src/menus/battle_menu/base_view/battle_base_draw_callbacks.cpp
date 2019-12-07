#include "battle_base_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBaseViewWidget(const MenuDrawEvent* event) {
    u8* byte_DC3930  = (u8*)0xDC3930;
    u32* dword_DC3BB0 = (u32*)0xDC3BB0;
    u32* dword_DC3AA0 = (u32*)0xDC3AA0;
    u32* dword_DC1F44 = (u32*)0xDC1F44;

    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/

    std::vector<std::string> names = { PARTY_1_STATE_NAME, PARTY_2_STATE_NAME, PARTY_3_STATE_NAME };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (CURRENT_PARTY_MEMBER_ARRAY[partyIdx] != 0xFF) {
            auto dataWidget = getChild(getChild(menuWidget, BATTLE_BASE_WIDGET_NAME), names[partyIdx]);
            enableWidget(dataWidget);

            u8 characterID = getCharacterRecordIndex(partyIdx);
            auto name = &(CHARACTER_RECORD_ARRAY[characterID].character_name);
            updateText(getChild(dataWidget, PARTY_DATA_NAME), (const char*)name);
            updateTextColor(getChild(dataWidget, PARTY_DATA_NAME), COLOR_WHITE);
            if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID && getMenuState(event->menu) != BATTLE_INACTIVE) {
                auto nameColor = COLOR_WHITE;
                if (((*dword_DC1F44 >> 3) & 1) != 0)
                    nameColor = COLOR_GRAY;

                updateTextColor(getChild(dataWidget, PARTY_DATA_NAME), nameColor);
            }

            i32 atbValue = PARTY_STRUCT_ARRAY[partyIdx].atbTimer;
            updateBarLength((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), atbValue >> 10);
            updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), -2146402240);
            if (atbValue == 0xFFFF) {
                if (*BATTLE_ACTIVE_ACTOR_ID == partyIdx || getMenuState(event->menu) == BATTLE_CMD_STATE) {
                    u16* word_DC3BE4 = (u16*)0xDC3BE4;
                    i32 colorMask = ((*word_DC3BE4 / 2) << 8) | (*word_DC3BE4 << 16);
                    updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), colorMask);
                }
            }

            i32 limitValue = PARTY_STRUCT_ARRAY[partyIdx].limitGuage;
            updateBarLength((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), limitValue >> 10);
            updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), -2139086768);
            if (limitValue == 0xFF00) {
                u32* dword_DC1F44 = (u32*)0xDC1F44;
                u32* dword_91E970 = (u32*)0x91E970;
                i32 colorMask = dword_91E970[(*dword_DC1F44 >> 1) & 7];
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), colorMask);
            }
            else if (gAiActorVariables[partyIdx].statusMask & STS_FURY) {
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), 0x80000080);
            }
            else if (gAiActorVariables[partyIdx].statusMask & STS_SADNESS) {
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), 0x80800000);
            }
            updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), -2146402240);


            auto isDead = (PLAYER_FLAG_COPIES[partyIdx].flags & 1);

            u16 maxHP = PARTY_STRUCT_ARRAY[partyIdx].maxHP;
            auto displayHP = dword_DC3AA0[partyIdx] >> 8;
            auto HPWidget = getChild(dataWidget, PARTY_DATA_HP);
            updateSegment(
                (ResourceBarWidget*)getChild(HPWidget, std::string("BAR")),
                (i16)byte_DC3930[partyIdx],
                (i16)maxHP,
                (i16)(dword_DC3BB0[partyIdx] >> 8)
            );
            updateResourceBarColor(
                (ResourceBarWidget*)getChild(HPWidget, std::string("BAR")),
                0x800080FF
            );
            updateNumber(getChild(HPWidget, std::string("MAX")), maxHP);
            updateNumber(getChild(HPWidget, std::string("CURRENT")), displayHP);
            if (isDead) {
                updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_RED);
            }
            else if (displayHP < (maxHP / 2)){
                updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_GREEN);
            }
            updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_WHITE);

            u32* dword_DC3A68 = (u32*)0xDC3A68;
            u32* byte_DC3A90 = (u32*)0xDC3A90;
            u16 maxMP = PARTY_STRUCT_ARRAY[partyIdx].maxMP;
            auto displayMP = dword_DC3A68[partyIdx] >> 8;
            auto MPWidget = getChild(dataWidget, PARTY_DATA_MP);
            updateSegment(
                (ResourceBarWidget*)getChild(MPWidget, std::string("BAR")),
                (i16)byte_DC3A90[partyIdx],
                (i16)maxMP,
                (i16)(dword_DC3A68[partyIdx] >> 8)
            );
            updateResourceBarColor(
                (ResourceBarWidget*)getChild(MPWidget, std::string("BAR")),
                0x800080FF
            );
            //updateNumber(getChild(MPWidget, std::string("MAX")), maxMP);
            updateNumber(getChild(MPWidget, std::string("CURRENT")), displayMP);
            if (isDead) {
                updateNumberColor(getChild(MPWidget, std::string("CURRENT")), COLOR_RED);
            }
            else if (displayMP < (maxMP / 2)) {
                updateNumberColor(getChild(MPWidget, std::string("CURRENT")), COLOR_GREEN);
            }
            updateNumberColor(getChild(MPWidget, std::string("CURRENT")), COLOR_WHITE);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_BASE_WIDGET_NAME), names[partyIdx]));
    }
}

void handleActorReady(const MenuDrawEvent* event) {
    u8* byte_DC2069 = (u8*)0xDC2069;
    u8* byte_DC207B = (u8*)0xDC207B;
    u8* byte_DC2084 = (u8*)0xDC2084;

    if (!*byte_DC2084) {
        auto actorIdx = getActorCycleTop();
        if (actorIdx != -1 && !*byte_DC2069 && !*byte_DC207B && (getMenuState(event->menu) == BATTLE_INACTIVE)) {
            if (actorIdx < 4) {
                *BATTLE_ACTIVE_ACTOR_ID = actorIdx;
                setActiveCursorIndex(event->menu, BATTLE_CMD_STATE, actorIdx);
                setMenuState(event->menu, BATTLE_CMD_STATE);
            }
            else {
                *ENEMY_ACTOR_IDX = actorIdx - 4;
                setMenuState(event->menu, BATTLE_MANIP_STATE);
            }
        }

        if (*byte_DC2069 && (actorIdx == 255 || actorIdx != *BATTLE_ACTIVE_ACTOR_ID)) {
            setMenuState(event->menu, BATTLE_INACTIVE);
        }
        if (*byte_DC207B && (actorIdx == 255 || actorIdx != (*ENEMY_ACTOR_IDX + 4))) {
            setMenuState(event->menu, BATTLE_INACTIVE);
        }
    }
}

void handleUpdateInputActive(const MenuDrawEvent* event) {
    if (*BATTLE_PAUSED)
        return;

    *ACCEPTING_BATTLE_INPUT = (sub_41AB67(128) != 0);
}

void handleTopDisplayString(const MenuDrawEvent* event) {
    u8* battleTextActive = (u8*)0xDC38BC;
    u16* stringBufferIdx = (u16*)0xDC38C0;
    u8* stringCommandIdx = (u8*)0xDC38EC;
    u16* stringActionIdx = (u16*)0xDC38F0;
    u8* actionDisplayActive = (u8*)0xDC38E8;
    char argSubBuffer[512] = {};

    auto topDisplayWidget = getChild(getChild(event->menu->menuWidget, BATTLE_BASE_WIDGET_NAME), TOP_STRING_DISPLAY);
    if (*battleTextActive) {
        enableWidget(topDisplayWidget);
        if (*stringBufferIdx < 256) {
            auto baseStr = gContext.gameStrings.battle_texts.get_string(*stringBufferIdx);
            handleBattleStrSubstitions(&(argSubBuffer[0]), baseStr);
            updateText(getChild(topDisplayWidget, "TXT"), &(argSubBuffer[0]));
        }
        else {
            u16 gStringBufferIdx = *stringBufferIdx - 256;
            auto baseStr = srGetStrFromGlobalBuffer(gStringBufferIdx);
            handleBattleStrSubstitions(&(argSubBuffer[0]), baseStr);
            updateText(getChild(topDisplayWidget, "TXT"), &(argSubBuffer[0]));
        }
    }
    else if (*actionDisplayActive) {
        enableWidget(topDisplayWidget);
        auto textToDraw = gContext.attacks.get_element(assemblekey(*stringCommandIdx, *stringActionIdx)).attackName.unicode();
        updateText(getChild(topDisplayWidget, "TXT"), textToDraw); // Will need some code to center the text
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

}
