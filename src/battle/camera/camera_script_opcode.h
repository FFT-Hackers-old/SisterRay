#ifndef CAMERA_SCRIPT_OPCODE_H
#define CAMERA_SCRIPT_OPCODE_H

#include <SisterRay/SisterRay.h>
#include "../../sr_named_registry.h"

SISTERRAY_API void initCameraScriptOpCodes();

class SrPosCameraOpCodeRegistry : public SrNamedResourceRegistry<SRPFNCAMSCRIPTOPCODEHANDLER, std::string> {
public:
    SrPosCameraOpCodeRegistry();
};

class SrLookAtCameraOpCodeRegistry : public SrNamedResourceRegistry<SRPFNCAMSCRIPTOPCODEHANDLER, std::string> {
public:
    SrLookAtCameraOpCodeRegistry();
};


#endif // !ANIMATION_OPCODE_H
