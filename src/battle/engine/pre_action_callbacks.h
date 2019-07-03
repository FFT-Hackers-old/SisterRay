#ifndef PRE_ACTION_CALLBACKS_H
#define PRE_ACTION_CALLBACKS_H

#include <SisterRay/SisterRay.h>

void runPreActionScripts(BattleQueueEntry* poppedAction, u8 issuingActorID);
void initDamageContext(BattleQueueEntry* poppedAction, u8 issuingActorID);
void setCommandData(BattleQueueEntry* poppedAction, u8 issuingActorID);
void handleToad(BattleQueueEntry* poppedAction, u8 issuingActorID);
void setAllFlag(BattleQueueEntry* poppedAction, u8 issuingActorID);
void prepareMimedAction(BattleQueueEntry* poppedAction, u8 issuingActorID);
#endif // !PRE_ACTION_CALLBACKS_H
