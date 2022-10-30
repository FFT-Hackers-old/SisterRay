#ifndef CAMERA_H
#define CAMERA_H

#include <SisterRay/SisterRay.h>

void srRunCameraScripts(u32 cameraScriptIdx, u32 spellEffectCtx);
void srAdvanceBattleCamera(u32 cameraScriptIdx);
void srHandleSpecialCamera();
u16 getCameraAction();
#endif // !RENDERING_H
