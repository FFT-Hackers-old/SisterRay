#ifndef ANIMATION_SCRIPT_PARSER_H
#define ANIMATION_SCRIPT_PARSER_H

#include <SisterRay/SisterRay.h>
#include "animation_script_opcode.h"
#include "animation_player.h"

void runAnimationScript(u8 actorID, u8** ptrToScriptTable);

#endif // !ANIMATION_SCRIPT_PARSER_H
