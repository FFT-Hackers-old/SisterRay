#ifndef BATTLE_EVENTS_H
#define BATTLE_EVENTS_H

#include <SisterRay/SisterRay.h>
#include "event.h"

typedef struct {
    DamageCalcStruct* damageContext;
} CommandSetupEvent;

typedef struct {

} SelectCommandEvent;

typedef struct {
    DamageCalcStruct* damageContext;
    BattleQueueEntry* poppedAction;
    u16 issuingActorID;
    AIBattleContext* battleAIContext;
} ActionContextEvent;
#endif // !BATTLE_EVENT_H
