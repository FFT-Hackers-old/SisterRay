#ifndef ACTION_QUEUE_POP_H
#define ACTION_QUEUE_POP_H

#include <SisterRay/SisterRay.h>
#include "pre_action_callbacks.h"
#include "post_action_callbacks.h"

#pragma pack(push, 1)
typedef struct {
    u8 field_0;
    u8 attackerID;
    u8 commandIdx;
    u16 relAttackIdx;
    u16 targetMask;
    u8 pad;
} MimeAction;
#pragma pack(pop)

#define G_MIME_ACTIONS ((MimeAction*)(0x9A9ADD))

#endif // !ACTION_QUEUE_POP_H
