#include "battle_model_utils.h"

/*Assemble the key used to look for animations in the game, for easy copying*/
std::string assembleEnemyModelKey(u16 idx) {
    std::string numString;
    if (idx < 10) {
        numString = std::string("00") + std::to_string(idx);
    }
    else if (idx < 100) {
        numString = std::string("0") + std::to_string(idx);
    }
    else {

        numString = std::to_string(idx);
    }
    return std::string("ENEMY") + numString + std::string(".DAT");
}
