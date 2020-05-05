#ifndef BATTLE_MODEL_STATE_INTERFACE_H
#define BATTLE_MODEL_STATE_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID);
SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID);
SISTERRAY_API BattleModel* getActiveModelFromArray(u8 arrayIdx);
SISTERRAY_API ModelRotationData* getBattleModelRotationData(u8 actorID);

SISTERRAY_API void addModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation);;
SISTERRAY_API void addPlayerModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation, const SrAnimation weaponAnimation);
SISTERRAY_API void addAnimationScript(const char* modName, u16 modIdx, const char* modelName, u8* script, u16 scriptLength);
SISTERRAY_API u16 getSrPlayerAnimScriptID(const char* modName, u16 relativeModIdx, const char* modelName);

SISTERRAY_API void setModelVanish(u8 actorID);
SISTERRAY_API void setModelAppaer(u8 actorID);
SISTERRAY_API void setActorIdleAnim(u8 actorID, u16 animID);
SISTERRAY_API void setActorHurtAnim(u8 actorID, u16 animScriptIdx);

#endif // !BATTLE_MODEL_STATE_INTERFACE_H
