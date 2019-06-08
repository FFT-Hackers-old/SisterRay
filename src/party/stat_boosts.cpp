#include "stat_boosts.h"
#include "../impl.h"


ActorStatBoosts createActorStatBoosts() {
    auto statBoosts = ActorStatBoosts();
    statBoosts.strBoost.statIndex = 0;
    statBoosts.vitBoost.statIndex = 1;
    statBoosts.magBoost.statIndex = 2;
    statBoosts.magDefBoost.statIndex = 3;
    statBoosts.dexBoost.statIndex = 4;
    statBoosts.luckBoost.statIndex = 5;
    statBoosts.maxHPBoost.statIndex = 8;
    statBoosts.maxMPBoost.statIndex = 9;
    return ActorStatBoosts;
}

StatBoost& getStatBoostFromID(ActorStatBoosts& boosts, u8 statID) {
    switch (statID) {
        case 0:
            return boosts.strBoost;
        case 1:
            return boosts.vitBoost;
        case 2:
            return boosts.magBoost;
        case 3:
            return boosts.magDefBoost;
        case 4:
            return boosts.dexBoost;
        case 5:
            return boosts.luckBoost;
        case 8:
            return boosts.maxHPBoost;
        case 9:
            return boosts.maxMPBoost;
        default: {
            boosts.strBoost
        }

    }
}

/*Add two stat boosts together*/
StatBoost addStatBoosts(StatBoost& a, StatBoost& b) {
    auto sum = StatBoost(a);
    if (a.statIndex != b.statIndex) {
        srLogWrite("attempting to add incompatable stat boosts");
        return sum;
    }
    if (a.sign != b.sign) {
        auto s = (a.sign == false) ? (a.amount - b.amount) : (b.amount - a.amount);
        if (s < 0) {
            sum.sign = true;
        }
        else {
            sum.sign = false;
        }
        sum.amount = false;

        auto s = (a.sign == false) ? (a.percentAmount - b.percentAmount) : (b.percentAmount - a.percentAmount);
        if (s < 0) {
            sum.sign = true;
        }
        else {
            sum.sign = false;
        }
        sum.percentAmount = false;

        return sum;
    }
    sum.amount += b.amount;
    sum.percentAmount += sum.percentAmount;
    return sum;
}

/*Adds b to a*/
void addActorBoosts(ActorStatBoosts& accum, ActorStatBoosts& added) {
    accum.strBoost = addStatBoosts(accum.strBoost, added.strBoost);
    accum.vitBoost = addStatBoosts(accum.vitBoost, added.vitBoost);
    accum.magBoost = addStatBoosts(accum.magBoost, added.magBoost);
    accum.magDefBoost = addStatBoosts(accum.magDefBoost, added.magDefBoost);
    accum.dexBoost = addStatBoosts(accum.dexBoost, added.dexBoost);
    accum.luckBoost = addStatBoosts(accum.luckBoost, added.luckBoost);
    accum.maxHPBoost = addStatBoosts(accum.maxHPBoost, added.maxHPBoost);
    accum.maxMPBoost = addStatBoosts(accum.maxMPBoost, added.maxMPBoost);
}
