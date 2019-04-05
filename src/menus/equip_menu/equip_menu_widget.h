#ifndef EQUIP_MENU_WIDGET
#define EQUIP_MENU_WIDGET

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"

// by using this namespace in a mod, you may access the Widgets
// in your own callbacks to mutate them however you like
namespace EquipWidgetNames {
    //Names of TOP LEVEL WIDGETS
    const std::string EQUIP_MENU_NAME = "EQUIP_MENU_WIDGET";
    const std::string CHAR_DATA_WIDGET_NAME = "CHAR_DATA_WIDGET";
    const std::string GEAR_DESC_WIDGET_NAME = "GEAR_DESC_WIDGET";
    const std::string GEAR_SLOTS_WIDGET_NAME = "GEAR_SLOTS_WIDGET";
    const std::string STAT_DIFF_WIDGET_NAME = "STAT_DIFF_WIDGET";
    const std::string GEAR_LIST_WIDGET_NAME = "GEAR_LIST_WIDGET";
    //Names of CHAR_DATA children
    const std::string GEAR_SLOT_1_NAME = "WPN_SLOT_NAME";
    const std::string GEAR_SLOT_2_NAME = "ARM_SLOT_NAME";
    const std::string GEAR_SLOT_3_NAME = "ACC_SLOW_NAME";
    const std::string EQUIPPED_WEAPON = "EQUIPPED_WEAPON";
    const std::string EQUIPPED_ARMOR = "EQUIPPED_ARMOR";
    const std::string EQUIPPED_ACC = "EQUIPPED_ACC";
    const std::string CHAR_DATA_BOX_NAME = "CHAR_DATA_BOX";
    const std::string PORTRAIT_WIDGET_NAME = "CHAR_DATA_PORTRAIT";
    const std::string HPBAR_WIDGET_NAME = "CHAR_DATA_HPBAR";
    //Names of GEAR_DESC children
    const std::string GEAR_DESCRIPTION = "GEAR_DESCRIPTION";
    const std::string GEAR_DESC_BOX = "GEAR_DESC_BOX";
    //Names of GEAR_SLOTS children
    const std::string GEAR_SLOTS = "GEAR_SLOTS";
    const std::string GEAR_GROWTH = "GEAR_GROWTH";
    const std::string SLOTS_NAME = "SLOTS_NAME";
    const std::string GROWTH_NAME = "GROWTH_NAME";
    const std::string GEAR_SLOTS_BOX = "GEAR_SLOTS_BOX";
    //Names of all stat diff Widgets
    const std::string STAT_NAME_1 = "STAT_NAME_1";
    const std::string STAT_NAME_2 = "STAT_NAME_2";
    const std::string STAT_NAME_3 = "STAT_NAME_3";
    const std::string STAT_NAME_4 = "STAT_NAME_4";
    const std::string STAT_NAME_5 = "STAT_NAME_5";
    const std::string STAT_NAME_6 = "STAT_NAME_6";
    const std::string STAT_NAME_7 = "STAT_NAME_7";
    const std::string STAT_VALUE_1 = "STAT_VALUE_1";
    const std::string STAT_VALUE_2 = "STAT_VALUE_2";
    const std::string STAT_VALUE_3 = "STAT_VALUE_3";
    const std::string STAT_VALUE_4 = "STAT_VALUE_4";
    const std::string STAT_VALUE_5 = "STAT_VALUE_5";
    const std::string STAT_VALUE_6 = "STAT_VALUE_6";
    const std::string STAT_VALUE_7 = "STAT_VALUE_7";
    const std::string ARROW_1 = "STAT_VALUE_1";
    const std::string ARROW_2 = "STAT_VALUE_2";
    const std::string ARROW_3 = "STAT_VALUE_3";
    const std::string ARROW_4 = "STAT_VALUE_4";
    const std::string ARROW_5 = "STAT_VALUE_5";
    const std::string ARROW_6 = "STAT_VALUE_6";
    const std::string ARROW_7 = "STAT_VALUE_7";
    const std::string NEW_STAT_VALUE_1 = "N_STAT_VALUE_1";
    const std::string NEW_STAT_VALUE_2 = "N_STAT_VALUE_2";
    const std::string NEW_STAT_VALUE_3 = "N_STAT_VALUE_3";
    const std::string NEW_STAT_VALUE_4 = "N_STAT_VALUE_4";
    const std::string NEW_STAT_VALUE_5 = "N_STAT_VALUE_5";
    const std::string NEW_STAT_VALUE_6 = "N_STAT_VALUE_6";
    const std::string NEW_STAT_VALUE_7 = "N_STAT_VALUE_7";
    const std::string STAT_DIFF_BOX = "STAT_DIFF_BOX";
    //Names of all gear list Widgets;
    const std::string GEAR_LIST_BOX = "GEAR_LIST_BOX";
    const std::string EQUIP_LIST = "EQUIP_LIST";

}

Widget* initEquipMenuWidget();
void initCharDataWidget(Widget* mainWidget);
void initGearMateriaSlotWidget(Widget* mainWidget);
void initGearDescWidget(Widget* mainWidget);
void initStatDiffWidget(Widget* mainWidget);

#endif
