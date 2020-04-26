#include "action_queue.h"


#define G_ACTION_QUEUE         ((BattleQueueEntry*)0x9A988C)
#define G_MINI_QUEUE           ((BattleQueueEntry*)0x9A9A8C)
void initializeActionQueue() {
    for (u8 queueIdx = 0; queueIdx < 64; queueIdx++)// NullActionQueue
        G_ACTION_QUEUE[queueIdx].entryPriority = 0xFF;

    for (u8 miniQueueIdx = 0; miniQueueIdx < 10; miniQueueIdx++)
        G_MINI_QUEUE[miniQueueIdx].entryPriority = 0xFF;
}
typedef void(*pfnsub43258A)(BattleQueueEntry*);
#define enqueueBattleAction      ((pfnsub43258A)0x43258A)
/*Rewrite this function to expect an ABSOLUTE instead of relative id when executing enemy attacks*/
SISTERRAY_API void srEnqueueBattleAction(BattleQueueEntry queueEntry) {
    enqueueBattleAction(&queueEntry);
}
