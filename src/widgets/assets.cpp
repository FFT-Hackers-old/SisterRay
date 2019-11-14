#include "assets.h"
#include "../impl.h"

void drawScrollerWidget(ScrollerWidget* scrollerPortrait) {
    drawScrollerParams params = {
        scrollerPortrait->viewSize,
        scrollerPortrait->maxRows,
        scrollerPortrait->baseRows,
        scrollerPortrait->position1,
        scrollerPortrait->position2,
        scrollerPortrait->position3,
        scrollerPortrait->position4
    };
    renderSideScroller((i32)&params, scrollerPortrait->priority);
}

ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name) {
    ScrollerWidget* widget = (ScrollerWidget*)createWidget(name, sizeof(ScrollerWidget), &kScrollerWidgetClass);
    widget->viewSize = params.viewSize;
    widget->baseRows = params.baseRow;
    widget->maxRows = params.maxRows;
    widget->position1 = params.position1;
    widget->position2 = params.position2;
    widget->position3 = params.position3;
    widget->position4 = params.position4;
    return widget;
}

bool isScrollerWidget(Widget* widget) {
    return ((widget->klass == &kScrollerWidgetClass));
}

const WidgetClass* ScrollerWidgetKlass() {
    return &kScrollerWidgetClass;
}


SISTERRAY_API void srNewPortraitWidget(Widget* parent, drawPortraitParams params, char* name) {
    auto strName = std::string(name);
    auto portraitWidget = createPortraitWidget(params, strName);
    addChildWidget(parent, (Widget*)portraitWidget, strName);
}

SISTERRAY_API void setPortraitParams(drawPortraitParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->partyIndex = partyIndex;
    params->priority = priority;
}

void drawPortraitWidget(PortraitWidget* portraitWidget) {
    displayPortrait(
        portraitWidget->widget.xCoordinate,
        portraitWidget->widget.yCoordinate,
        portraitWidget->partyIndex,
        portraitWidget->priority
    );
}

PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name) {
    PortraitWidget* widget = (PortraitWidget*)createWidget(name, sizeof(PortraitWidget), &kPortraitWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->partyIndex = params.partyIndex;
    widget->priority = params.priority;
    return widget;
}

bool isPortraitWidget(Widget* widget) {
    return ((widget->klass == &kPortraitWidgetClass));
}

const WidgetClass* PortraitWidgetKlass() {
    return &kPortraitWidgetClass;
}

SISTERRAY_API void updatePortraitPartyIndex(Widget* widget, u8 portraitIndex) {
    if (isPortraitWidget(widget)) {
        auto typedPtr = (PortraitWidget*)widget;
        typedPtr->partyIndex = portraitIndex;
    }
    else {
        //YA DONE MESSED UP
    }
}

SISTERRAY_API void srNewHPBarWidget(Widget* parent, drawHPBarParams params, char* name) {
    auto strName = std::string(name);
    auto HPBarWidget = createHPBarWidget(params, strName);
    addChildWidget(parent, (Widget*)HPBarWidget, strName);
}
SISTERRAY_API void setHPBarParams(drawHPBarParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->partyIndex = partyIndex;
    params->priority = priority;
}

void drawHPBarWidget(HPBarWidget* hpBarWidget) {
    renderHPAndStatus(
        hpBarWidget->widget.xCoordinate,
        hpBarWidget->widget.yCoordinate,
        hpBarWidget->partyIndex,
        hpBarWidget->priority
    );
}

HPBarWidget* createHPBarWidget(drawHPBarParams params, std::string name) {
    HPBarWidget* widget = (HPBarWidget*)createWidget(name, sizeof(HPBarWidget), &kHPBarWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->partyIndex = params.partyIndex;
    widget->priority = params.priority;
    return widget;
}

const WidgetClass* HPBarWidgetKlass() {
    return &kHPBarWidgetClass;
}

bool isHPBarWidget(Widget* widget) {
    return ((widget->klass == &kHPBarWidgetClass));
}

SISTERRAY_API void updateHPBarPartyIndex(Widget* widget, u8 partyIndex) {
    if (isHPBarWidget(widget)) {
        auto typedPtr = (HPBarWidget*)widget;
        typedPtr->partyIndex = partyIndex;
    }
    else {
        //YA DONE MESSED UP
    }
}

SISTERRAY_API void srNewSlotsWidget(Widget* parent, drawSlotsParams params, char* name) {
    auto strName = std::string(name);
    auto slotsWidget = createSlotsWidget(params, strName);
    addChildWidget(parent, (Widget*)slotsWidget, strName);
}

SISTERRAY_API void setSlotsParams(drawSlotsParams* params, i32 xCoordinate, i32 yCoordinate, u8* materiaSlotData) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->materiaSlotData = materiaSlotData;
}

void drawSlotsWidget(SlotsWidget* slotsWidget) {
    if (slotsWidget->materiaData) {
        for (auto slotIdx = 0; slotIdx < 8; slotIdx++) {
            if (slotsWidget->materiaData[slotIdx].item_id == 0xFFFF)
                continue;
            if (!slotsWidget->materiaSlotsData[slotIdx])
                continue;
            auto materiaType = getMateriaColorType(slotsWidget->materiaData[slotIdx].item_id);
            drawComplexGameAsset(
                slotsWidget->widget.xCoordinate + 28 * slotIdx,
                slotsWidget->widget.yCoordinate,
                128,
                32,
                16,
                16,
                materiaType,
                1,
                0,
                .2f
            );
        }
    }
    if (slotsWidget->materiaSlotsData) {
        renderMateriaSlots(
            slotsWidget->widget.xCoordinate,
            slotsWidget->widget.yCoordinate,
            (i32)(slotsWidget->materiaSlotsData)
        );
    }
}

SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name) {
    SlotsWidget* widget = (SlotsWidget*)createWidget(name, sizeof(SlotsWidget), &kSlotsWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->materiaSlotsData = params.materiaSlotData;
    widget->materiaData = params.materiaData;
    return widget;
}

bool isSlotsWidget(Widget* widget) {
    return ((widget->klass == &kSlotsWidgetClass));
}

const WidgetClass* SlotsWidgetKlass() {
    return &kSlotsWidgetClass;
}

SISTERRAY_API void updateMateriaSlots(Widget* widget, u8* materiaSlotsData) {
    if (isSlotsWidget(widget)) {
        auto typedPtr = (SlotsWidget*)widget;
        typedPtr->materiaSlotsData = materiaSlotsData;
    }
    else {
        //YA DONE MESSED UP
    }
}

SISTERRAY_API void updateMateriaData(Widget* widget, MateriaInventoryEntry* materiaData) {
    if (isSlotsWidget(widget)) {
        auto typedPtr = (SlotsWidget*)widget;
        typedPtr->materiaData= materiaData;
    }
    else {
        //YA DONE MESSED UP
    }
}

SISTERRAY_API void srNewSimpleAssetWidget(Widget* parent, DrawSimpleAssetParams params, char* name) {
    auto strName = std::string(name);
    auto simpleAssetWidget = createSimpleGameAssetWidget(params, strName);
    addChildWidget(parent, (Widget*)simpleAssetWidget, strName);
}

SISTERRAY_API void setSimpleAssetParams(DrawSimpleAssetParams* params, i32 xCoordinate, i32 yCoordinate, u8 assetType, color assetColor, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->arrowCode = assetType;
    params->arrowColor = assetColor;
    params->arrowPriority = priority;
}

void drawSimpleAssetWidget(SimpleAssetWidget* simpleAssetWidget) {
    gameDrawAsset(
        simpleAssetWidget->widget.xCoordinate,
        simpleAssetWidget->widget.yCoordinate,
        simpleAssetWidget->code,
        simpleAssetWidget->arrowColor,
        simpleAssetWidget->priority
    );
}

SimpleAssetWidget* createSimpleGameAssetWidget(DrawSimpleAssetParams params, std::string name) {
    SimpleAssetWidget* widget = (SimpleAssetWidget*)createWidget(name, sizeof(SimpleAssetWidget), &kSimpleAssetWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->code = params.arrowCode;
    widget->priority = params.arrowPriority;
    return widget;
}

bool isSimpleAssetWidget(Widget* widget) {
    return ((widget->klass == &kSimpleAssetWidgetClass));
}

const WidgetClass* SimpleAssetWidgetKlass() {
    return &kSimpleAssetWidgetClass;
}

DrawSimpleAssetParams Sign(i32 xCoordinate, i32 yCoordinate, color color, float priority, bool isPositive) {
    DrawSimpleAssetParams sign;
    if (isPositive) {
        sign = { xCoordinate, yCoordinate, 0xB3, color, priority };
        return sign;
    }
    sign = { xCoordinate, yCoordinate, 0xE1, color, priority };
    return sign;
}
DrawSimpleAssetParams Percent(i32 xCoordinate, i32 yCoordinate, color color, float priority) {
    DrawSimpleAssetParams percent = { xCoordinate, yCoordinate, 0xD3, color, priority };
    return percent;
}
DrawSimpleAssetParams Arrow(i32 xCoordinate, i32 yCoordinate, color color, float priority) {
    DrawSimpleAssetParams arrow = { xCoordinate, yCoordinate, 0xDA, color, priority };
    return arrow;
}

DrawSimpleAssetParams Cross(i32 xCoordinate, i32 yCoordinate, color color, float priority) {
    DrawSimpleAssetParams arrow = { xCoordinate, yCoordinate, 0xD5, color, priority };
    return arrow;
}


SISTERRAY_API void srNewGameAssetWidget(Widget* parent, DrawGameAssetParams params, char* name) {
    auto strName = std::string(name);
    auto gameAssetWidget = createGameAssetWidget(params, strName);
    addChildWidget(parent, (Widget*)gameAssetWidget, strName);
}

void drawGameAssetWidget(GameAssetWidget* gameAssetWidget) {
    drawComplexGameAsset(gameAssetWidget->widget.xCoordinate, gameAssetWidget->widget.yCoordinate,
        gameAssetWidget->unk1, gameAssetWidget->unk2, gameAssetWidget->unk3,
        gameAssetWidget->unk4, gameAssetWidget->unk5, gameAssetWidget->unk6,
        gameAssetWidget->unk7, gameAssetWidget->priority);
}

GameAssetWidget* createGameAssetWidget(DrawGameAssetParams params, std::string name) {
    GameAssetWidget* widget = (GameAssetWidget*)createWidget(name, sizeof(GameAssetWidget), &kGameAssetWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->unk1 = params.unk1;
    widget->unk2 = params.unk2;
    widget->unk3 = params.unk3;
    widget->unk4 = params.unk4;
    widget->unk5 = params.unk5;
    widget->unk6 = params.unk6;
    widget->unk7 = params.unk7;
    widget->priority = params.priority;
    return widget;
}

bool isGameAssetWidget(Widget* widget) {
    return ((widget->klass == &kGameAssetWidgetClass));
}

const WidgetClass* GameAssetWidgetKlass() {
    return &kGameAssetWidgetClass;
}

SISTERRAY_API void setGameAssetParams(DrawGameAssetParams* params, i32 xCoordinate, i32 yCoordinate, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8, i32 arg9, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->unk1 = arg3;
    params->unk2 = arg4;
    params->unk3 = arg5;
    params->unk4 = arg6;
    params->unk5 = arg7;
    params->unk6 = arg8;
    params->unk7 = arg9;
    params->priority = priority;
}


SISTERRAY_API void updateAssetType(Widget* widgetToUpdate, i32 type) {
    if (isGameAssetWidget(widgetToUpdate)) {
        auto typedPtr = (GameAssetWidget*)widgetToUpdate;
        typedPtr->unk5= type;
    }
    else {
        srLogWrite("attempting to update assetType field of an invalid Widget type");
    }
}

SISTERRAY_API void transformAsset(Widget* widgetToUpdate, i32 type1, i32 type2, i32 type3, i32 type4) {
    if (isGameAssetWidget(widgetToUpdate)) {
        auto typedPtr = (GameAssetWidget*)widgetToUpdate;
        typedPtr->unk1 = type1;
        typedPtr->unk2 = type2;
        typedPtr->unk3 = type3;
        typedPtr->unk4 = type4;
    }
    else {
        srLogWrite("attempting to update assetType field of an invalid Widget type");
    }
}

DrawGameAssetParams MateriaSphere(i32 xCoordinate, i32 yCoordinate, i32 sphereColor, float priority) {
    DrawGameAssetParams sphere = { xCoordinate, yCoordinate, 128, 32, 16, 16, sphereColor, 0, 0, priority};
    return sphere;
}

DrawGameAssetParams MateriaStar(i32 xCoordinate, i32 yCoordinate, i32 starColor, float priority, bool shaded) {
    if (shaded) {
        DrawGameAssetParams star = { xCoordinate, yCoordinate, 144, 32, 16, 16, starColor, 0, 0, priority };
        return star;
    }
    DrawGameAssetParams star = { xCoordinate, yCoordinate, 144, 48, 16, 16, starColor, 0, 0, priority };
    return star;
}

DrawGameAssetParams ItemIcon(i32 xCoordinate, i32 yCoordinate, i32 iconType, float priority) {
    DrawGameAssetParams icon = { xCoordinate, yCoordinate, 16 * (iconType & 1) + 96, 16 * (iconType / 2) + 112, 16, 16, 1, 0, 0, priority };
    return icon;
}

DrawGameAssetParams AllArrow(i32 xCoordinate, i32 yCoordinate, float priority) {
    DrawGameAssetParams allArw = { xCoordinate, yCoordinate, 128, 8, 8, 8, 2, 0, 0, priority };
    return allArw;
}

DrawGameAssetParams BarBorder(i32 xCoordinate, i32 yCoordinate, float priority) {
    DrawGameAssetParams barBorder = { xCoordinate, yCoordinate, 16, 0, 40, 10, 1, 0, 0, priority };
    return barBorder
}

DrawGameAssetParams BarrierBarBorder(i32 xCoordinate, i32 yCoordinate, float priority) {
    DrawGameAssetParams barrierBarBorder = { xCoordinate, yCoordinate, 160, 16, 38, 12, 1, 0, 0, priority };
    return barrierBarBorder
}

#define MENU_ASSET_TIME 0
#define MENU_ASSET_WAIT 1
#define MENU_ASSET_CHAR 2
#define MENU_ASSET_HP 3
#define MENU_ASSET_MP 4
#define MENU_ASSET_LIMIT 5
#define MENU_ASSET_BARRIER 6

DrawGameAssetParams BattleTexts(i32 xCoordinate, i32 yCoordinate, float priority, int type) {
    i32 unk1 = 0;
    i32 unk2 = 0;
    i32 unk3 = 0;
    switch (type) {
        case 0: {
            unk1 = 96;
            unk2 = 8;
            unk3 = 24;
            break;
        }
        case 1: {
            unk1 = 112;
            unk2 = 0;
            unk3 = 24;
            break;
        }
        case 2: {
            unk1 = 128;
            unk2 = 16;
            unk3 = 26;
            break;
        }
        case 3: {
            unk1 = 80;
            unk2 = 0;
            unk3 = 16;
            break;
        }
        case 4: {
            unk1 = 96;
            unk2 = 0;
            unk3 = 16;
            break;
        }
        case 5: {
            unk1 = 56;
            unk2 = 0;
            unk3 = 24;
            break;
        }
        case 6: {
            unk1 = 56;
            unk2 = 8;
            unk3 = 38;
            break;
        }
    }

    DrawGameAssetParams menuTextAsset = { xCoordinate, yCoordinate, unk1, unk2, unk3, 5, 1, 0, 0, priority };
    return menuTextAsset
}



void setStarShaded(Widget* widgetToUpdate, bool shaded) {
    if (isGameAssetWidget(widgetToUpdate)) {
        auto typedPtr = (GameAssetWidget*)widgetToUpdate;
        if (typedPtr->unk1 == 144) {
            if (shaded) {
                typedPtr->unk2 = 32;
                return;
            }
            typedPtr->unk2 = 48;
        }
    }
    else {
        srLogWrite("attempting to update assetType field of an invalid Widget type");
    }
}

void updateItemIcon(Widget* widgetToUpdate, i32 iconType) {
    if (isGameAssetWidget(widgetToUpdate)) {
        auto typedPtr = (GameAssetWidget*)widgetToUpdate;
        typedPtr->unk1 = 16 * (iconType & 1) + 96;
        typedPtr->unk2 = 16 * (iconType / 2) + 112;
    }
    else {
        srLogWrite("attempting to update assetType field of an invalid Widget type");
    }
}

void drawBarWidget(BarWidget* barWidget) {
    gameDrawBar(
        barWidget->widget.xCoordinate,
        barWidget->widget.yCoordinate,
        barWidget->fraction,
        barWidget->unk1,
        barWidget->unk2,
        barWidget->priority
    );
}

const WidgetClass* BarWidgetKlass();
BarWidget* createBarWidget(DrawBarParams params, std::string name) {
    BarWidget* widget = (BarWidget*)createWidget(name, sizeof(BarWidget), &kBarWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->fraction = params.fraction;
    widget->unk1 = params.unk1;
    widget->unk2 = params.unk2;
    widget->priority = params.priority;
    return widget;
}

bool isBarWidget(Widget* widget) {
    return ((widget->klass == &kBarWidgetClass));
}

const WidgetClass* BarWidgetKlass() {
    return &kBarWidgetClass;
}
