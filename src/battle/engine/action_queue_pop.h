#ifndef ACTION_QUEUE_POP_H
#define ACTION_QUEUE_POP_H

#include <SisterRay/SisterRay.h>
#include "pre_action_callbacks.h"
#include "post_action_callbacks.h"

void srHandlePoppedAction(BattleQueueEntry* poppedAction);
void preActionHandles(BattleQueueEntry* poppedAction, u8 issuingActorID);
void postActionHandles(BattleQueueEntry* poppedAction, u8 issuingActorID);
void registerActionPopCallbacks();

#endif // !ACTION_QUEUE_POP_H
