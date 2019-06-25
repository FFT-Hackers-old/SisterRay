#ifndef BATTLE_H
#define BATTLE_H

#include <SisterRay/SisterRay.h>
#include <cstdio>

typedef struct {
    u8 sizeScale;
    u8 idleAnim;
    u8 damagedAnim;
} scriptAnimDataCpy;

i16 srLoadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void));
i32 srExecuteAIScript(i32 actorIndex, i32 scriptID, i32 unkInt);
i32 srExecuteFormationScripts();
i32  enqueueScriptAction(i16 characterID, i16 commandIndex, i16 relAttackIndex);
void* transformEnemyCommand();

void dispatchAutoActions(i32 partyIndex, i32 actionType);
u16 getActionToDispatch(const SrAutoAction& action);
u16 setTargetMask(u8 partyIndex, const SrAutoAction& action);

#endif
