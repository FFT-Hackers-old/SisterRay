#include "battle_base_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

#define computeResourceDisplays ((PFNSR_VOIDSUB)0x6DCBAA)

void drawBaseViewWidget(const MenuDrawEvent* event) {
    u8* byte_DC3930  = (u8*)0xDC3930;
    u32* dword_DC3BB0 = (u32*)0xDC3BB0;
    u32* dword_DC3AA0 = (u32*)0xDC3AA0;
    u32* dword_DC1F44 = (u32*)0xDC1F44;

    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/

    computeResourceDisplays();
    std::vector<std::string> names = { PARTY_1_STATE_NAME, PARTY_2_STATE_NAME, PARTY_3_STATE_NAME };
    for (u8 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (gContext.party.isSlotEnabled(partyIdx)) {
            auto dataWidget = getChild(getChild(menuWidget, BATTLE_BASE_WIDGET_NAME), names[partyIdx]);
            enableWidget(dataWidget);

            const char* characterName = nullptr;
            if (gContext.battleActors.isActorSummon(1)) {
                characterName = getCommandAction(CMD_SUMMON, 2).attackName.str();
            }
            else {
                characterName = gContext.party.getActivePartyCharacter(partyIdx).gameCharacter->character_name;
            }

            updateText(getChild(dataWidget, PARTY_DATA_NAME), characterName);
            updateTextColor(getChild(dataWidget, PARTY_DATA_NAME), COLOR_WHITE);
            if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID && getMenuState(event->menu) != BATTLE_INACTIVE) {
                auto nameColor = COLOR_WHITE;
                if (((*dword_DC1F44 >> 3) & 1) != 0)
                    nameColor = COLOR_GRAY;
                updateTextColor(getChild(dataWidget, PARTY_DATA_NAME), nameColor);
            }

            u16 atbValue = getActivePartyMember(partyIdx).gamePartyMember->atbTimer;
            updateBarLength((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), atbValue >> 10);
            updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), 0x80108040);
            if (atbValue == 0xFFFFu) {
                if (*BATTLE_ACTIVE_ACTOR_ID == partyIdx && getMenuState(event->menu) != BATTLE_INACTIVE) {
                    u16* word_DC3BE4 = (u16*)0xDC3BE4;
                    u32 colorMask = (word_DC3BE4[0] / 2 << 8) | (word_DC3BE4[0] << 16);
                    updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), colorMask);
                }
                else {
                    updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), 0x80808020);
                }
            }

            u16 limitValue = getActivePartyMember(partyIdx).gamePartyMember->limitGuage;
            updateBarLength((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), limitValue >> 10);
            updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), -2139086768);
            if (limitValue == 0xFF00) {
                u32* dword_DC1F44 = (u32*)0xDC1F44;
                u32* dword_91E970 = (u32*)0x91E970;
                u32 colorMask = dword_91E970[(*dword_DC1F44 >> 1) & 7];
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), colorMask);
            }
            else if (gAiActorVariables[partyIdx].statusMask & STS_FURY) {
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), 0x80000080);
            }
            else if (gAiActorVariables[partyIdx].statusMask & STS_SADNESS) {
                updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_LIMIT), std::string("BAR")), 0x80800000);
            }
            //updateBarColor((BarWidget*)getChild(getChild(dataWidget, PARTY_DATA_ATB), std::string("BAR")), -2146402240);


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

#define popCharacterActionQueue   ((PFNSR_INTSUB)0x437481)
void handleActorReady(const MenuDrawEvent* event) {
    u8* byte_DC2069 = (u8*)0xDC2069;
    u8* byte_DC207B = (u8*)0xDC207B;
    u8* byte_DC2084 = (u8*)0xDC2084;
    u16* word_9AAD04 = (u16*)0x9AAD04;

    if (!*byte_DC2084) {
        auto actorIdx = getActorCycleTop();
        srLogWrite("Actor Cycle Top: %i", actorIdx);
        srLogWrite("Game Menu State: %i, expected inactive: %i", getMenuState(event->menu), BATTLE_INACTIVE);
        if (actorIdx != 0xFF && !*byte_DC2069 && !*byte_DC207B && (getMenuState(event->menu) == BATTLE_INACTIVE)) {
            if (actorIdx < 4) {
                srLogWrite("Activating Actor Cycle Top: %i", actorIdx);
                if (gContext.party.isSlotEnabled(actorIdx)) {
                    srLogWrite("Activating Actor Cycle Top: %i", actorIdx);
                    *BATTLE_ACTIVE_ACTOR_ID = actorIdx;
                    setActiveCursorIndex(event->menu, BATTLE_CMD_STATE, actorIdx);
                    setMenuState(event->menu, BATTLE_CMD_STATE);
                }
                else {
                    popCharacterActionQueue(actorIdx);
                    srLogWrite("Deactivating invalid Actor Cycle Top: %i", getActorCycleTop());
                    setMenuState(event->menu, BATTLE_INACTIVE);
                    //*word_9AAD04 &= (~(1 << actorIdx));
                }
            }
            else {
                srLogWrite("SETTING MENU STATE TO MANIPULATE");
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
