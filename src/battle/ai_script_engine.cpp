#include "ai_script_engine.h"
#include "../impl.h"

/*This is the handler for the 86 AI script opcode, rewritten to work correctly for new scenes*/
i32 getMPCost(i32 attackID) {
    u16 MPCost;

    MPCost = 0;
    if (attackID < 0xFFFF) {
        const AttackData& attackData = getAttackDataPtr(attackID);
        MPCost = attackData.MPCost;
    }
    return MPCost;
}

/*This now gets the MP cost of an enemy spell, or player spells with enemy indexes*/
const AttackData& getAttackDataPtr(i32 attackID) {
    const auto& attack = getCommandAction(CMD_ENEMY_ACTION, attackID).attackData;
    return attack;
}
