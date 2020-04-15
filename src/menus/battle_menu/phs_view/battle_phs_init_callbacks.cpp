#include "battle_phs_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattlePHSViewWidget(const MenuInitEvent* event) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;
    auto battlePHSView = createWidget(BATTLE_PHS_WIDGET_NAME);

    boxParams = {
        0,
        340,
        640,
        140,
        0.4f
    };
    boxWidget = createBoxWidget(boxParams, PHS_GRID_BOX);
    addChildWidget(battlePHSView, (Widget*)boxWidget, PHS_GRID_BOX);

    std::vector<std::string> names = { PARTY_1_PHS_GRID, PARTY_2_PHS_GRID, PARTY_3_PHS_GRID };
    for (u32 idx = 0; idx < names.size(); idx++) {
        auto characterChoice = getStateCursor(event->menu, BATTLE_PHS_STATE, idx);
        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_PHS_STATE, &battlePHSUpdater, 42, 360, allocatePHSRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battlePHSView, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, battlePHSView, BATTLE_PHS_WIDGET_NAME);
}

Widget* allocatePHSRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto charWidget = createWidget(name);
    moveWidget(charWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(charWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    drawPortraitParams portraitParams = { xCoordinate, yCoordinate, 0, 0.1f };
    addChildWidget(charWidget, (Widget*)createPortraitWidget(portraitParams, std::string("PIC")), std::string("PIC"));
    return charWidget;
}

void battlePHSUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto inactiveParty = getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->partySwapBuffer;
    if (inactiveParty[flatIndex] == 0xFF) {
        disableWidget(getChild(widget, std::string("TXT")));
        disableWidget(getChild(widget, std::string("PIC")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    enableWidget(getChild(widget, std::string("PIC")));
    updateText(getChild(widget, std::string("TXT")), getCharacterRecordWithID(inactiveParty[flatIndex])->character_name);
    updatePortraitPartyIndex(getChild(widget, std::string("PIC")), inactiveParty[flatIndex]);
    updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
}
