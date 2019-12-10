#ifndef ANIMATION_SCRIPT_PARSER_H
#define ANIMATION_SCRIPT_PARSER_H

#include <SisterRay/SisterRay.h>
#include "animation_script_opcode.h"


void runAnimationScript(u16 actorID, u8** ptrToScriptTable);
void srPlayModelAnimation(u16 actorID);
void srHandleAnimateModel(u16 actorID);

#endif // !ANIMATION_SCRIPT_PARSER_H
