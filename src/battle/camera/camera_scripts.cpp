#include "camera_scripts.h"
#include "../../impl.h"

static const u32 camDatOffset = 0x801A0000;
static const u32 camDat1Count = 0x168;

CameraScript buildCameraScript(u8* start, u8* rawBuffer) {
    auto script = CameraScript();
    return script;
    
}

SrCameraScriptRegistry::SrCameraScriptRegistry(void* camDatBuffer) {
    u8* buffStart = (u8*)camDatBuffer;
    u32* dwordReader = (u32*)camDatBuffer;
    u32* positionTablePtr = (u32*)(*dwordReader - camDatOffset);
    u32* lookAtTablePtr = (u32*)*((dwordReader + 1) - camDatOffset);

    u32* specialTablePtr = (u32*)((*dwordReader + 2) - camDatOffset);
    u32* specialTabelLookAt = (u32*)((*dwordReader + 3) - camDatOffset);
    u8* rawBuffer = (u8*)(dwordReader + 4);


    u32* positionTableRdr = positionTablePtr;
    u32* lookAtTableRdr = lookAtTablePtr;
    for (auto idx = 0; idx < camDat1Count; idx++) {
        auto actionScripts = SrActionCameraScripts();
        actionScripts.variationCount = 3;
        auto& variationsArray = actionScripts.variationScripts;
        for (auto varIdx = 0; varIdx < actionScripts.variationCount; varIdx++) {
            auto cameraScript = SrCameraScript();
            auto tableIdx = (u32)(positionTableRdr + (3 * idx) + varIdx);
            cameraScript.posScript = buildCameraScript((u8*)(buffStart + (positionTableRdr[tableIdx] - camDatOffset)), rawBuffer);
            cameraScript.lookAtScript = buildCameraScript((u8*)(buffStart + (lookAtTableRdr[tableIdx] - camDatOffset)), rawBuffer);
        }
    }

}
