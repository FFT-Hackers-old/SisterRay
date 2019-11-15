#include "battle_base_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBaseViewWidget(const BattleDrawEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto baseWidget = createWidget(BATTLE_BASE_WIDGET_NAME);

    boxParams = {
        0,
        340,
        680,
        140,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, BASE_BOX_NAME);
    addChildWidget(baseWidget, (Widget*)boxWidget, BASE_BOX_NAME);

    std::vector<std::string> names = { PARTY_1_STATE_NAME, PARTY_2_STATE_NAME, PARTY_3_STATE_NAME };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto baseRowWidget = allocateBaseRow(names[idx].c_str(), 28, 32 * idx + 348);
        addChildWidget(baseRowWidget, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, baseWidget, BATTLE_BASE_WIDGET_NAME);
}

Widget* allocateBaseRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto baseDataWidget = createWidget(name);
    moveWidget(baseDataWidget, xCoordinate, yCoordinate);
    
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(baseDataWidget, (Widget*)createTextWidget(textParams, std::string("NAME")), std::string("NAME"));

    Widget atbWidget = allocateBarWidget(std::string("ATB").c_str(), xCoordinate + 532, yCoordinate);
    addChildWidget(baseDataWidget, atbWidget, std::string("ATB"));

    Widget limitWidget = allocateBarWidget(std::string("LIMIT").c_str(), xCoordinate + 454, yCoordinate);
    addChildWidget(baseDataWidget, limitWidget, std::string("LIMIT"));

    Widget barrierWidget = allocateBarriersWidget(std::string("BARRIERS").c_str(), xCoordinate + 168, yCoordinate);
    addChildWidget(baseDataWidget, barrierWidget, std::string("BARRIERS");

    Widget hpWidget = allocateHPResourceWidget(std::string("HP").c_str(), xCoordinate + 260, yCoordinate);
    addChildWidget(baseDataWidget, hpWidget, std::string("HP"));

    Widget mpWidget = allocateMPResourceWidget(std::string("MP").c_str(), xCoordinate + 388, yCoordinate);
    addChildWidget(baseDataWidget, mpWidget, std::string("MP"));

    return baseDataWidget;
}

Widget* allocateBarWidget(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto atbDataWidget = createWidget(name);
    moveWidget(atbDataWidget, xCoordinate, yCoordinate);

    DrawGameAssetParams barBorder = BarBorder(xCoordinate, yCoordinate, 0.6f);
    addChildWidget(barriersDataWidget, (Widget*)createGameAssetWidget(barBorder, std::string("BORDER")), std::string("BORDER"));

    DrawBarParams atbBarParams = { xCoordinate, yCoordinate, 0, 12, -2139095040, 0.6f };
    addChildWidget(atbDataWidget, (Widget*)createBarWidget(atbBarParams, std::string("BAR")), std::string("BAR"));
    return atbDataWidget;
}

Widget* allocateHPResourceWidget(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto resourceWidget = createWidget(name);
    moveWidget(resourceWidget, xCoordinate, yCoordinate);

    DrawResourceBarParams resourceBar = { xCoordinate, yCoordinate + 16, 120, 2, 0, 0, 0, 0, 0.6f };
    addChildWidget(resourceWidget, (Widget*)createResourceBarWidget(resourceBar, std::string("BAR")), std::string("BAR"));

    DrawNumberParams numberParams = { xCoordinate, yCoordinate, 0, 4, COLOR_WHITE, 0.6f };
    addChildWidget(baseDataWidget, (Widget*)createTextWidget(textParams, std::string("CURRENT")), std::string("CURRENT"));

    DrawNumberParams numberParams = { xCoordinate + 64, yCoordinate, 0, 4, COLOR_WHITE, 0.6f };
    addChildWidget(baseDataWidget, (Widget*)createTextWidget(textParams, std::string("MAX")), std::string("MAX"));

    return resourceWidget;
}

Widget* allocateMPResourceWidget(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto resourceWidget = createWidget(name);
    moveWidget(resourceWidget, xCoordinate, yCoordinate);

    DrawResourceBarParams resourceBar = { xCoordinate, yCoordinate + 16, 60, 2, 0, 0, 0, 0, 0.6f };
    addChildWidget(resourceWidget, (Widget*)createResourceBarWidget(resourceBar, std::string("BAR")), std::string("BAR"));

    DrawNumberParams numberParams = { xCoordinate, yCoordinate, 0, 3, COLOR_WHITE, 0.6f };
    addChildWidget(baseDataWidget, (Widget*)createTextWidget(textParams, std::string("CURRENT")), std::string("CURRENT"));

    DrawNumberParams numberParams = { xCoordinate + 64, yCoordinate, 0, 3, COLOR_WHITE, 0.6f };
    addChildWidget(baseDataWidget, (Widget*)createTextWidget(textParams, std::string("MAX")), std::string("MAX"));

    return resourceWidget;
}

Widget* allocateBarriersWidget(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto barriersDataWidget = createWidget(name);
    moveWidget(barriersDataWidget, xCoordinate, yCoordinate);

    DrawGameAssetParams barrierBorder = BarrierBarBorder(xCoordinate, yCoordinate, 0.6f);
    addChildWidget(barriersDataWidget, (Widget*)createGameAssetWidget(barrierBorder, std::string("BARRIERS_BORDER")), std::string("BARRIERS_BORDER"));

    DrawBarParams barrierBarParams = { xCoordinate, yCoordinate, 0, 8, -2139095040, 0.6f };
    addChildWidget(barriersDataWidget, (Widget*)createBarWidget(barrierBarParams, std::string("BARRIER_BAR")), std::string("BARRIER_BAR"));
    DrawBarParams mBarrierBarParams = { xCoordinate, yCoordinate, 0, 8, -2139078656, 0.6f };
    addChildWidget(barriersDataWidget, (Widget*)createBarWidget(mBarrierBarParams, std::string("MBARRIER_BAR")), std::string("MBARRIER_BAR"));

    return barriersDataWidget
}
