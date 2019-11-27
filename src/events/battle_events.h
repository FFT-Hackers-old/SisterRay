#ifndef BATTLE_EVENTS_H
#define BATTLE_EVENTS_H

#include <SisterRay/SisterRay.h>
#include "event.h"

typedef struct {
    DamageCalcStruct* damageContext;
} CommandSetupEvent;

typedef struct {
    Menu*  menuObect;
    EnabledCommandStruct* command;
} SelectCommandEvent;

typedef struct {
    DamageCalcStruct* damageContext;
    BattleQueueEntry* poppedAction;
    u16 issuingActorID;
    AIBattleContext* battleAIContext;
} ActionContextEvent;

typedef struct {
    GameAnimationScriptContext* scriptContext;
    u8* scriptPtr;
    u8 currentScriptIdx;
    BattleModelState* battleModelDataArray;
    u8 actorID;
} AnimScriptEvent;
#endif // !BATTLE_EVENT_H
