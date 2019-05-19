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

const AttackData& getAttackDataPtr(i32 attackID) {
    //TODO relocate player data and change this logic
    AttackData* playerAbilityData = (AttackData*)(0xDB9690);

    if (attackID < 256)
        const auto& attackData = playerAbilityData[attackID];
    const auto& attackData = gContext.enemyAttacks.get_element(std::string(std::to_string(attackID))).attackData;

    return attackData;
}
