#ifndef CAMERA_SCRIPTS_H
#define CAMERA_SCRIPTS_H

#include <SisterRay/SisterRay.h>
#include <map>
#include <unordered_set>
#include "../../sr_named_registry.h"

typedef std::vector<u8> CameraScript;

/*Raw buffer size is used for memcpying the relevant information when a model attempts to seek it*/
typedef struct {
    u32 posLength;
    u32 lookAtLength;
    CameraScript posScript;
    CameraScript lookAtScript;
} SrCameraScript;

/*This data lives in a registry, and is copied into the right place when the game is allocating models for a given battle
  This is necessary to avoid the game attempting to */
typedef struct {
    u16 variationCount;
    SrNamedResourceRegistry<SrCameraScript, std::string> variationScripts;
} SrActionCameraScripts;

SrActionCameraScripts createSrCameraScripts(void* camDatBuffer);

class SrCameraScriptRegistry : public SrNamedResourceRegistry<SrActionCameraScripts, std::string> {
public:
    SrCameraScriptRegistry(void* camDatBuffer);
    SrCameraScriptRegistry() : SrNamedResourceRegistry<SrActionCameraScripts, std::string>() {}
    u32 getMemoryBufferSize(const std::string& name);
};

CameraScript camScriptFromBuffer(u8* animScriptStart, u16 animScriptLength, u16* trueScriptLength);
void initCameraScripts(void* camDatBuffer);
void* srInitializeAnimScriptsData(const char* filename, ModelAAHeader* aaHeader);

#endif // !CAMERA_SCRIPTS_H