#include "inventory_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"

/* This Module contains code which initializes the Widget that represents the equip menu internally
   You can mutate this widget prior to draw by registered your listeners to the "OnEquipMenuDraw" event
   The Widget is also mutated by listeners registered to the Equip-Menu's input Handlers.
*/

using namespace InventoryWidgetNames;

void initViewChoiceWidget(const InventoryInitEvent* event) {
    const char * menuText;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    PortraitWidget* portraitWidget;

    auto viewChoiceWidget = createWidget(VIEW_CHOICE_WIDGET_NAME);

    boxParams = {
       menuWindowConfig[0].drawDistance1,
       menuWindowConfig[0].drawDistance2,
       menuWindowConfig[0].drawDistance3,
       menuWindowConfig[0].drawDistance4,
       0.111f
    };

    boxWidget = createBoxWidget(boxParams, VIEW_CHOICE_BOX);
    addChildWidget(viewChoiceWidget, (Widget*)boxWidget, VIEW_CHOICE_BOX);

    std::vector<std::string> listNames = { INV_VIEW_1, INV_VIEW_2, INV_VIEW_3 };
    for (int column = 0; column < 3; ++column) {
        setTextParams(&textParams, 98 * column + 57, 17, gContext.gameStrings.inventory_menu_texts.get_string(column), COLOR_WHITE, 0.1f);
        auto textChild = createTextWidget(textParams, listNames[column]);
        addChildWidget(viewChoiceWidget, (Widget*)textChild, listNames[column]);
    }
    // sub_6FA12F(0, 102, 640, 372);

    addChildWidget(mainWidget, (Widget*)viewChoiceWidget, VIEW_CHOICE_WIDGET_NAME);
}

void initCharViewWidget(const InventoryInitEvent* event) {
    const char * menuText;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    PortraitWidget* portraitWidget;

    auto partyViewWidget = createWidget(PARTY_VIEW_WIDGET_NAME);

    boxParams = {
        (u16)0,
        (u16)96,
        (u16)300,
        (u16)384,
        0.1f
    };

    boxWidget = createBoxWidget(boxParams, PARTY_VIEW_BOX);
    addChildWidget(partyViewWidget, (Widget*)boxWidget, PARTY_VIEW_BOX);


    std::vector<std::string> portraitNames = { PORTRAIT_1_NAME, PORTRAIT_2_NAME, PORTRAIT_3_NAME };
    std::vector<std::string> HPBarNames = { HPBAR_1_NAME, HPBAR_2_NAME, HPBAR_3_NAME };
    for (int currentPartyMember = 0; currentPartyMember < 3; ++currentPartyMember) {
        if ((CURRENT_PARTY_MEMBER_ARRAY)[currentPartyMember] != 0xFF) {  
            drawPortraitParams portraitParams = { 37, 120*currentPartyMember + 116, currentPartyMember, 0.1f };
            portraitWidget = createPortraitWidget(portraitParams, portraitNames[currentPartyMember]);
            addChildWidget(partyViewWidget, (Widget*)portraitWidget, portraitNames[currentPartyMember]);

            drawHPBarParams hpBarParams = { 133, 120*currentPartyMember +126, currentPartyMember, 0.1f };
            auto HPBarWidget = createHPBarWidget(hpBarParams, HPBarNames[currentPartyMember]);
            addChildWidget(partyViewWidget, (Widget*)HPBarWidget, HPBarNames[currentPartyMember]);
        }
    }

    addChildWidget(mainWidget, (Widget*)partyViewWidget, PARTY_VIEW_WIDGET_NAME);
}

//Handles the base display
void initItemViewWidget(const InventoryInitEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto itemViewWidget = createWidget(ITEM_VIEW_WIDGET_NAME);

    boxParams = {
        menuWindowConfig[2].drawDistance1,
        menuWindowConfig[2].drawDistance2,
        menuWindowConfig[2].drawDistance3,
        menuWindowConfig[2].drawDistance4,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, ITEM_VIEW_BOX);
    addChildWidget(itemViewWidget, (Widget*)boxWidget, ITEM_VIEW_BOX);

    // displayVisibleItemIcon(343, 37 * visibleItem + 9 * baseColumnIndex + 105, itemID, 0, .1f); //Function needs to be overwritten to display correct thing
    // gameDrawAsset(548, 37 * visibleItem + 9 * baseColumnIndex + 114, 213, (u8)textColor, .1f);

    auto normalItemViewWidget = createWidget(NORMAL_ITEM_VIEW_NAME);
    gridParams = { INVENTORY_MENU_NAME.c_str(), 1, &inventoryRowUpdater, 373, 109, &allocateInventoryRow, 0 };
    gridWidget = createGridWidget(gridParams, ITEM_VIEW_NAMES);
    addChildWidget(normalItemViewWidget, (Widget*)gridWidget, ITEM_VIEW_NAMES);

    addChildWidget(itemViewWidget, normalItemViewWidget, NORMAL_ITEM_VIEW_NAME);

    //This second cursor tracks the inventory when custom sort is activated, and is toggled in the draw menus//
    auto customItemViewWidget = createWidget(CUSTOM_ITEM_VIEW_NAME);

    gridParams = { INVENTORY_MENU_NAME.c_str(), 5, &inventoryRowUpdater, 373, 109, &allocateInventoryRow, 0 };
    gridWidget = createGridWidget(gridParams, CUSTOM_SORT_VIEW_NAMES, TextWidgetKlass());
    addChildWidget(customItemViewWidget, (Widget*)gridWidget, CUSTOM_SORT_VIEW_NAMES);

    addChildWidget(itemViewWidget, customItemViewWidget, CUSTOM_ITEM_VIEW_NAME);

    addChildWidget(mainWidget, itemViewWidget, ITEM_VIEW_WIDGET_NAME);
}

Widget* allocateInventoryRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto inventoryWidget = createWidget(name);
    moveWidget(inventoryWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(inventoryWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawNumberParams numberParams = { xCoordinate + 177, yCoordinate, 0, 2, COLOR_WHITE, 0.1f };
    addChildWidget(inventoryWidget, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
    return inventoryWidget;
}

/*Update the current view of the inventory based on the relevant cursor*/
void inventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto itemID = gContext.inventory->get_resource(flatIndex).item_id;
    auto textWidget = getChild(widget, std::string("TXT"));
    auto numberWidget = getChild(widget, std::string("AMT"));

    if (itemID != 0xFFFF) {
        enableWidget(textWidget);
        auto textColor = usableInInventoryMenu(itemID) ? COLOR_GRAY : COLOR_WHITE;
        const char* name = getNameFromItemID(itemID);
        srLogWrite("displaying name of item: %s", name);
        updateText(textWidget, name);
        updateTextColor(textWidget, textColor);


        enableWidget(numberWidget);
        auto numberColor = usableInInventoryMenu(itemID) ? COLOR_GRAY : COLOR_WHITE;
        auto itemQuantity = gContext.inventory->get_resource(flatIndex).quantity;
        updateNumber(numberWidget, itemQuantity);
        updateNumberColor(numberWidget, numberColor);
    }
    else {
        disableWidget(textWidget);
        disableWidget(numberWidget);
    }
}


void keyItemsViewWidget(const InventoryInitEvent* event) {
    auto keyItemsChoice = getStateCursor(event->menuObject, 3);

    drawGridParams gridParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto keyItemsWidget = createWidget(KEY_ITEMS_VIEW_NAME);

    boxParams = {
        (i16)0,
        (i16)102,
        (i16)640,
        (i16)372,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, KEY_ITEMS_BOX);
    addChildWidget(keyItemsWidget, (Widget*)boxWidget, KEY_ITEMS_BOX);

    gridParams = { INVENTORY_MENU_NAME.c_str(), 3, &keyItemViewNameUpdater, 53, 124, nullptr, 0};
    auto cursorListWidget = createGridWidget(gridParams, KEY_ITEMS_GRID, TextWidgetKlass());
    addChildWidget(keyItemsWidget, (Widget*)cursorListWidget, KEY_ITEMS_GRID);

    addChildWidget(mainWidget, keyItemsWidget, KEY_ITEMS_VIEW_NAME);
}

void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto keyItemIndex = (KEY_ITEMS_INVENTORY_PTR)[(u8)(flatIndex)];
    if (keyItemIndex != 0xFFFF) {
        enableWidget(widget);
        const char* name = gContext.gameStrings.key_item_names.get_string(flatIndex);
        updateText(widget, name);
        updateTextColor(widget, COLOR_WHITE);
    }
    else {
        disableWidget(widget);
    }
}

void itemDescriptionWidget(const InventoryInitEvent* event) {
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;

    auto itemDescWidget = createWidget(ITEM_DESC_WIDGET_NAME);

    boxParams = {
       menuWindowConfig[1].drawDistance1,
       menuWindowConfig[1].drawDistance2,
       menuWindowConfig[1].drawDistance3,
       menuWindowConfig[1].drawDistance4,
       0.2f
    };
    boxWidget = createBoxWidget(boxParams, ITEM_DESC_BOX);
    addChildWidget(itemDescWidget, (Widget*)boxWidget, ITEM_DESC_BOX);

    textParams = { 27, 64, "\xFF", COLOR_WHITE, 0.1f };
    textWidget = createTextWidget(textParams, ITEM_DESCRIPTION);
    addChildWidget(itemDescWidget, (Widget*)textWidget, ITEM_DESCRIPTION);

    addChildWidget(mainWidget, itemDescWidget, ITEM_DESC_WIDGET_NAME);
}

void arrangeTypeWidget(const InventoryInitEvent* event) {
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;

    auto arrangeTypeWidget = createWidget(ARRANGE_TYPE_WIDGET_NAME);

    boxParams = {
       (i16)0xDC,
       (i16)0x1A,
       (i16)0x91,
       (i16)0xE3,
       0.001f
    };
    boxWidget = createBoxWidget(boxParams, ARRANGE_TYPE_BOX);
    addChildWidget(arrangeTypeWidget, (Widget*)boxWidget, ARRANGE_TYPE_BOX);

    //Probably a good case to be made here for a new static Grid with no updater here
    std::vector<std::string> listNames = { SORT_TYPE_1, SORT_TYPE_2, SORT_TYPE_3, SORT_TYPE_4, SORT_TYPE_5, SORT_TYPE_6, SORT_TYPE_7, SORT_TYPE_8 };
    for (int sortType = 0; sortType < listNames.size(); ++sortType) {
        const char* fetchedDescription = gContext.gameStrings.inventory_menu_texts.get_string(sortType + 3);
        setTextParams(&textParams, boxParams.drawDistance1 + 13, boxParams.drawDistance2 + 26 * sortType + 13, fetchedDescription, COLOR_WHITE, 0.001f);
        auto textChild = createTextWidget(textParams, listNames[sortType]);
        addChildWidget(arrangeTypeWidget, (Widget*)textChild, listNames[sortType]);
    }

    addChildWidget(mainWidget, arrangeTypeWidget, ARRANGE_TYPE_WIDGET_NAME);
}

