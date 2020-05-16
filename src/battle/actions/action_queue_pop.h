#ifndef ACTION_QUEUE_POP_H
#define ACTION_QUEUE_POP_H

#include <SisterRay/SisterRay.h>

void srHandlePoppedAction(BattleQueueEntry* poppedAction);
void preActionHandles(ActionContextEvent& actionEvent);
void postActionHandles(ActionContextEvent& actionEvent);
void registerActionPopCallbacks();

#endif // !ACTION_QUEUE_POP_H
