#ifndef SR_PARTY_H
#define SR_PARTY_H

typedef struct {
    int sign;
    u16 statIndex;
    u16 amount;
    u16 percentAmount;
} StatBoost;

typedef struct {
    StatBoost strBoost;
    StatBoost vitBoost;
    StatBoost magBoost;
    StatBoost magDefBoost;
    StatBoost dexBoost;
    StatBoost luckBoost;
    StatBoost maxHPBoost;
    StatBoost maxMPBoost;
    StatBoost coverChance;
} ActorStatBoosts;

#endif // !SR_PARTY_H
