#include "stat_boosts.h"
#include "../impl.h"

ActorStatBoosts::ActorStatBoosts() {
    strBoost.statIndex = 0;
    strPenalty.statIndex = 0;
    vitBoost.statIndex = 1;
    vitPenalty.statIndex = 1;
    magBoost.statIndex = 2;
    magPenalty.statIndex = 2;
    magDefBoost.statIndex = 3;
    magDefPenalty.statIndex = 3;
    dexBoost.statIndex = 4;
    dexPenalty.statIndex = 4;
    luckBoost.statIndex = 5;
    luckPenalty.statIndex = 5;
    maxHPBoost.statIndex = 8;
    maxHPPenalty.statIndex = 8;
    maxMPBoost.statIndex = 9;
    maxMPPenalty.statIndex = 9;
}

/*Add two stat boosts together*/
StatBoost addStatBoosts(StatBoost& a, StatBoost& b) {
    auto sum = StatBoost(a);
    if (a.statIndex != b.statIndex) {
        srLogWrite("attempting to add incompatable stat boosts");
        return sum;
    }
    sum.amount += b.amount;
    sum.percentAmount += sum.percentAmount;
    return sum;
}

/*Adds b to a*/
void addActorBoosts(ActorStatBoosts& accum, ActorStatBoosts& added) {
    accum.strBoost = addStatBoosts(accum.strBoost, added.strBoost);
    accum.strPenalty = addStatBoosts(accum.strPenalty, added.strPenalty);
    accum.vitBoost = addStatBoosts(accum.vitBoost, added.vitBoost);
    accum.vitPenalty = addStatBoosts(accum.vitPenalty, added.vitPenalty);
    accum.magBoost = addStatBoosts(accum.magBoost, added.magBoost);
    accum.magPenalty = addStatBoosts(accum.magPenalty, added.magPenalty);
    accum.magDefBoost = addStatBoosts(accum.magDefBoost, added.magDefBoost);
    accum.magDefPenalty = addStatBoosts(accum.magDefPenalty, added.magDefPenalty);
    accum.dexBoost = addStatBoosts(accum.dexBoost, added.dexBoost);
    accum.dexPenalty = addStatBoosts(accum.dexPenalty, added.dexPenalty);
}
