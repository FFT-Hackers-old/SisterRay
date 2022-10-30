#include "camera_script_opcode.h"
#include "../../impl.h"


SrPosCameraOpCodeRegistry::SrPosCameraOpCodeRegistry() : SrNamedResourceRegistry<SRPFNCAMSCRIPTOPCODEHANDLER, std::string>() {

}

SrLookAtCameraOpCodeRegistry::SrLookAtCameraOpCodeRegistry() : SrNamedResourceRegistry<SRPFNCAMSCRIPTOPCODEHANDLER, std::string>() {

}

SISTERRAY_API void initCameraScriptOpCodes() {
    gContext.camLookAtOpCodes = SrLookAtCameraOpCodeRegistry();

    gContext.camPositionOpcodes = SrPosCameraOpCodeRegistry();
    srLogWrite("Registered %i Animation Script OpCodes", gContext.animScriptOpcodes.resourceCount());
}
