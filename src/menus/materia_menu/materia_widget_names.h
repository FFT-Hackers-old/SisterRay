#ifndef MATERIA_WIDGET_NAMES_H
#define MATERIA_WIDGET_NAMES_H

namespace MateriaWidgetNames {
    //Names of TOP LEVEL WIDGETS
    const std::string MATERIA_MENU_NAME = "MAT_MENU";
    const std::string CHAR_DATA_WIDGET_NAME = "CHAR_DATA";
    const std::string CHAR_DATA_BOX_NAME = "CHAR_DATA_BOX";
    const std::string MATERIA_GRID_WIDGET_NAME = "MATERIA_VIEW";
    const std::string MATERIA_DESC_WIDGET_NAME = "MATERIA_DESC";
    const std::string MATERIA_DATA_WIDGET_NAME = "PARTY_VIEW";
    const std::string CHECK_VIEW_WIDGET_NAME = "CHECK_VIEW";
    const std::string COMMAND_VIEW_WIDGET_NAME = "COMMAND_VIEW";
    const std::string SPELL_VIEW_WIDGET_NAME = "SPELL_VIEW";
    const std::string ARRANGE_WIDGET_NAME = "ARRANGE";
    //CHAR DATA WIDGETS//
    const std::string GEAR_SLOT_1_NAME = "WPN_SLOT";
    const std::string GEAR_SLOT_2_NAME = "ARM_SLOT";
    const std::string EQUIPPED_WEAPON = "EQUIPPED_WPN";
    const std::string GEAR_1_SLOTS = "1_GEAR_SLOTS";
    const std::string EQUIPPED_ARMOR = "EQUIPPED_ARM";
    const std::string GEAR_2_SLOTS = "2_GEAR_SLOTS";
    const std::string PORTRAIT_WIDGET_NAME = "CHAR_PORTRAIT";
    const std::string HPBAR_WIDGET_NAME = "CHAR_HPBAR";
    const std::string CHECK_NAME = "CHECK";
    const std::string ARRANGE_NAME = "ARRANGE";
    //MATERIA_VIEW_WIDGET//
    const std::string MATERIA_GRID = "MATERIA_GRID";
    const std::string MATERIA_GRID_NAMES = "GRID_NAMES";
    const std::string MATERIA_GRID_SPHERES = "GRID_SPHERES";
    const std::string MATERIA_GRID_BOX = "MATERIA_GRID_BOX";
    //MATERIA_DESC_WIDGET//
    const std::string MATERIA_DESC = "MATERIA_DESC";
    const std::string MATERIA_DESC_BOX = "MATERIA_DESC_BOX";
    //MATERIA_DATA_WIDGET//
    const std::string STANDARD_DISPLAY = "NAME_MAT";
    const std::string MATERIA_NAME = "NAME_MAT";
    const std::string MATERIA_ELEMENT = "ELMT_MAT";
    const std::string MATERIA_SPHERE = "SPHERE_MAT";
    const std::string MATERIA_STARS = "STARS_MAT";
    const std::string AP_TEXT = "AP_TEXT";
    const std::string NEXT_LVL_TEXT = "LVL_TEXT";
    const std::string CURRENT_AP = "CURR_AP";
    const std::string MASTERED = "MASTERED";
    const std::string TO_LEVEL_AP = "LVL_AP";
    const std::string ABILITIES_LIST_TXT = "ABL_TEXT";
    const std::string EQUIP_EFFECT_TXT = "EQUIP_TEXT";
    const std::string ABILITIES_LIST = "ABILITIES_LIST"; //static text array, limit 5
    const std::string INDEPENDENT_BOOSTS = "IND_BOOSTS";
    const std::string EQUIP_EFFECTS = "EQUIP_EFFECTS"; //Static numbers arrray, limt 6, contains 6 sub widgets each with 4 widgets, 1_name, 1_number, 1_percent, 1_sign
    const std::string STAT_BOOSTS = "STAT_BOOSTS"; //Static numbers array, limit 5, contains 5 sub_widgets each with 4 widgets, 1_stat, 1_number, 1_percent, 1_sign
    const std::string ESKILL_WIDGET = "ESKILL";
    //CHECK_VIEW_WIDGET//
    const std::string CHECK_VIEW_BOX = "CHECK_BOX";
    //COMMAND_VIEW_WIDGET//
    const std::string CMD_GRID = "CMD_GRID";
    const std::string CMD_GRID_BOX = "CMD_GRID_BOX";
    //SPELL_VIEW_WIDGET//
    const std::string SPELL_GRID = "SPELL_GRID";
    const std::string SUMMON_GRID = "SUMMON_GRID";
    const std::string ESKILL_GRID = "ESKILL_GRID";
}
#endif
