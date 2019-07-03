# #ifndef BATTLE_MODELS_API_H
#define BATTLE_MODELS_API_H

#include <SisterRay/SisterRay.h>
#include "animation_registry.h"
#include "animation_scripts.h"

SISTERRAY_API void addModelAnimation(const char* modelName, SrAnimation animation);
SISTERRAY_API void addAnimationScript(const char* modelName, u8* script, u16 scriptLength);
SISTERRAY_API void addPlayerModelAnimation(const char* modelName, const SrAnimation animation, const SrAnimation weaponAnimation);
#endif // !BATTLE_MODELS_API_H
