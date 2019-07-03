#ifndef POST_ACTION_CALLBACKS_H
#define POST_ACTION_CALLBACKS_H

#include <SisterRay/SisterRay.h>

void handleCounters(BattleQueueEntry* poppedAction, u8 issuingActorID);
void setMimeData(BattleQueueEntry* poppedAction, u8 issuingActorID);
void handleLuckySevens(BattleQueueEntry* poppedAction, u8 issuingActorID);
void handleAddedCut(BattleQueueEntry* poppedAction, u8 issuingActorID);
#endif 
