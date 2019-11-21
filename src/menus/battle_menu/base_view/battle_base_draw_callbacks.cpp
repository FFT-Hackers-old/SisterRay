#include "battle_base_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBaseViewWidget(const MenuDrawEvent* event) {
    u8* byte_DC3930  = (u8*)0xDC3930;
    u32* dword_DC3BB0 = (u32*)0xDC3BB0;
    u32* dword_DC3AA0 = (u32*)0xDC3AA0;

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

            i32 maxHP = PARTY_STRUCT_ARRAY[partyIdx].maxHP;
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
            auto isDead = (PLAYER_FLAG_COPIES[partyIdx].flags & 1);
            if (isDead) {
                updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_RED);
            }
            else if (displayHP < (maxHP / 2)){
                updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_GREEN);
            }
            updateNumberColor(getChild(HPWidget, std::string("CURRENT")), COLOR_WHITE);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_BASE_WIDGET_NAME), names[partyIdx]));
    }
}

void handleActorReady(const MenuDrawEvent* event) {
    u8* byte_DC2069 = (u8*)0xDC2069;
    u8* byte_DC207B = (u8*)0xDC207B;
    u8* byte_DC2084 = (u8*)0xDC2084;


    srLogWrite("RUNNING ACTOR ACTION READY CALLBACK");
    if (!*byte_DC2084) {
        auto actorIdx = getActorCycleTop();
        srLogWrite("ACTOR INDEX FROM CYCLE: %d", actorIdx);
        if (actorIdx != -1 && !*byte_DC2069 && !*byte_DC207B && (getMenuState(event->menu) == BATTLE_INACTIVE)) {
            if (actorIdx < 4) {
                srLogWrite("TRANSITIONING TO COMMMAND ACTIVE STATE");
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
