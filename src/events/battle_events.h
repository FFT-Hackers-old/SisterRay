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
    u8 actorID;
    u8* scriptPtr;
    AnimScriptContext* scriptContext;
    u8 currentScriptIdx;
    BattleModelState* battleModelState;
} AnimScriptEvent;

typedef struct {
    u8 actorID;
    u8 animationType;
    u8 animationEffectID;
    u8 commandIdx;
    u16 actionIdx;
    u16 targetMask;
} AnimEffectEvent;
#endif // !BATTLE_EVENT_H
