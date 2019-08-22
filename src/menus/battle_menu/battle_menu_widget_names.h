#ifndef BATTLE_MENU_WIDGET_NAMES
#define BATTLE_MENU_WIDGET_NAMES

#define BATTLE_INACTIVE 0xFFFFFFFF
#define BATTLE_COMD_TARGETING 0
#define BATTLE_CMD_STATE 1
#define SUMMON_BATTLE_STATE  7

namespace BattleMenuWidgetNames {
    const std::string BATTLE_MENU_NAME = std::string("BATTLE_MENU");
    const std::string BATTLE_MENU_WIDGET_NAME = std::string("BATTLE_MENU_WIDGET");

    //Command Names
    const std::string BATTLE_COMMAND_WIDGET_NAME = std::string("BATTLE_COMMAND");
    const std::string PARTY_1_CMD_GRID = std::string("1_CMD_GRID");
    const std::string PARTY_2_CMD_GRID = std::string("2_CMD_GRID");
    const std::string PARTY_3_CMD_GRID = std::string("3_CMD_GRID");
    const std::string CMD_GRID_BOX = std::string("CMD_BOX");

    //Names of TOP LEVEL WIDGETS
    const std::string BATTLE_SUMMON_WIDGET_NAME = std::string("BATTLE_SUMMON");
    const std::string PARTY_1_SUMMON_GRID = std::string("1_SPELL_GRID");
    const std::string PARTY_2_SUMMON_GRID = std::string("2_SPELL_GRID");
    const std::string PARTY_3_SUMMON_GRID = std::string("3_SPELL_GRID");
}
#endif
