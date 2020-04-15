#ifndef CHARACTER_MOD_H
#define CHARACTER_MOD_H

#include <SisterRay/SisterRay.h>
#include <string>


namespace SoldierWidgetNames {
    const std::string SOLDIER_WIDGET_NAME = "SOLDIER_WIDGET";
    const std::string SOLDIER_BOX_NAME = "SOLDIER_BOX";
    const std::string SOLDIER_GRID_NAME = "SOLDIER_GRID";
}

void enableSoldier(const EnableDefaultAbilitiesEvent* const event);
void loadCharMod();
void initCloudMenu();
void initSoldierCommandViewWidget(const MenuInitEvent* event);
Widget* allocateSoldierRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void soldierCommandUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void drawSoldierViewWidget(const MenuDrawEvent* event);
void cmdSoldierelectHandler(SelectCommandEvent* event);
void handleSelectSoldierAction(const MenuInputEvent* event);
#endif
