#ifndef ACTION_QUEUE_POP_H
#define ACTION_QUEUE_POP_H

#include <SisterRay/SisterRay.h>
#include "pre_action_callbacks.h"
#include "post_action_callbacks.h"

void srHandlePoppedAction(BattleQueueEntry* poppedAction);
void preActionHandles(ActionContextEvent actionEvent);
void postActionHandles(ActionContextEvent actionEvent);
void registerActionPopCallbacks();

#endif // !ACTION_QUEUE_POP_H
