#include "ai_script_engine.h"
#include "../impl.h"

/*This is the handler for the 86 AI script opcode, rewritten to work correctly for new scenes*/
i32 getMPCost(i32 attackID)
{
    u16 MPCost;

    MPCost = 0;
    if (attackID < 0xFFFF) {
        const AttackData& attackData = getAttackDataPtr(attackID);
        MPCost = attackData.MPCost;
    }
    return MPCost;
}

/*It would be ideal if we could find some way to get the command index here, as this raw index thing will not work with new relative system*/
const AttackData& getAttackDataPtr(i32 attackID) {
    //TODO relocate player data and change this logic
    AttackData* playerAbilityData = (AttackData*)(0xDB9690);

    if (attackID < 256)
        const auto& attack = playerAbilityData[attackID];
    const auto& attack = gContext.attacks.get_element(assemblekey(CMD_ENEMY_ACTION, attackID)).attackData;

    return attack;
}
