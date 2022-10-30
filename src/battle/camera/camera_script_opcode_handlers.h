#ifndef CAMERA_SCRIPT_OPCODE_HANDLERS_H
#define CAMERA_SCRIPT_OPCODE_HANDLERS_H

#include <SisterRay/SisterRay.h>
#include "camera_script_opcode.h"

OpCodeControlSequence nopCode(CamScriptEvent* srEvent);
OpCodeControlSequence pOpCodeD5(CamScriptEvent* srEvent);
OpCodeControlSequence pOpCodeD6(CamScriptEvent* srEvent);
OpCodeControlSequence pOpCodeD7(CamScriptEvent* srEvent);

#endif // !CAMER_SCRIPT_OPCODE_HANDLERS_H
