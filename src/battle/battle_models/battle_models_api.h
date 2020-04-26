#ifndef BATTLE_MODELS_API_H
#define BATTLE_MODELS_API_H

#include <SisterRay/SisterRay.h>
#include "animation_registry.h"
#include "animation_scripts.h"

SISTERRAY_API void addModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation);
SISTERRAY_API void addPlayerModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation, const SrAnimation weaponAnimation);
SISTERRAY_API void addAnimationScript(const char* modName, u16 modIdx, const char* modelName, u8* script, u16 scriptLength);

SISTERRAY_API u16 getSrPlayerAnimationIdx(const char* modName, u16 relativeModIdx, const char* modelName);
SISTERRAY_API u16 getSrPlayerAnimScriptID(const char* modName, u16 relativeModIdx, const char* modelName);
#endif // !BATTLE_MODELS_API_H
