#ifndef ANIMATION_SCRIPT_PARSER_H
#define ANIMATION_SCRIPT_PARSER_H

#include <SisterRay/SisterRay.h>
#include "../../events/battle_events.h"
#include "animation_script_opcode.h"


void srPlayModelAnimation(u16 actorID);
void srHandleAnimateModel(u16 actorID);

#endif // !ANIMATION_SCRIPT_PARSER_H
