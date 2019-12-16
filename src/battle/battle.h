#ifndef BATTLE_H
#define BATTLE_H

#include <SisterRay/SisterRay.h>
#include <cstdio>
#include "scene_globals.h"

typedef struct {
    u8 sizeScale;
    u8 idleAnim;
    u8 damagedAnim;
} scriptAnimDataCpy;

void  srLoadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void));
void srExecuteAIScript(i32 actorIndex, i32 scriptID, i32 unkInt);
i32 srExecuteFormationScripts();
i32  enqueueScriptAction(u8 characterID, u8 commandIndex, u16 relAttackIndex);

void dispatchAutoActions(u8 partyIndex, i32 actionType);
u16 getActionToDispatch(const SrAutoAction& action);
u16 setTargetMask(u8 partyIndex, const SrAutoAction& action);

#endif
