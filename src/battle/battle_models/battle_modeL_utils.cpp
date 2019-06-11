#include "battle_model_utils.h"

/*Assemble the key used to look for animations in the game, for easy copying*/
std::string assembleEnemyModelKey(u16 idx) {
    return std::string("ENEMY") + std::to_string(idx) + std::string(".DAT");
}
