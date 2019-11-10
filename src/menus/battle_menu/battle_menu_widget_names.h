#ifndef BATTLE_MENU_WIDGET_NAMES
#define BATTLE_MENU_WIDGET_NAMES

#define BATTLE_INACTIVE 0xFFFFFFFF
#define BATTLE_TARGETING_STATE 0
#define BATTLE_CMD_STATE 1
#define SUMMON_BATTLE_STATE  7
#define BATTLE_MAGIC_STATE 6

namespace BattleMenuWidgetNames {
    const std::string BATTLE_MENU_NAME = std::string("BATTLE_MENU");
    const std::string BATTLE_MENU_WIDGET_NAME = std::string("BATTLE_MENU_WIDGET");

    //Command Names
    const std::string BATTLE_COMMAND_WIDGET_NAME = std::string("BATTLE_COMMAND");
    const std::string PARTY_1_CMD_GRID = std::string("1_CMD_GRID");
    const std::string PARTY_2_CMD_GRID = std::string("2_CMD_GRID");
    const std::string PARTY_3_CMD_GRID = std::string("3_CMD_GRID");
    const std::string CMD_GRID_BOX = std::string("CMD_BOX");

    //Magic Names
    const std::string BATTLE_SPELL_WIDGET_NAME = std::string("BATTLE_SPELL");
    const std::string PARTY_1_SPELL_GRID = std::string("1_SPELL_GRID");
    const std::string PARTY_2_SPELL_GRID = std::string("2_SPELL_GRID");
    const std::string PARTY_3_SPELL_GRID = std::string("3_SPELL_GRID");

    //Summon Names
    const std::string BATTLE_SUMMON_WIDGET_NAME = std::string("BATTLE_SUMMON");
    const std::string PARTY_1_SUMMON_GRID = std::string("1_SUMM_GRID");
    const std::string PARTY_2_SUMMON_GRID = std::string("2_SUMM_GRID");
    const std::string PARTY_3_SUMMON_GRID = std::string("3_SUMM_GRID");


}
#endif
