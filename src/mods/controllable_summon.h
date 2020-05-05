#ifndef CONTROLLABLE_SUMMON_H
#define CONTROLLABLE_SUMMON_H

#include <SisterRay/SisterRay.h>

void loadControllableSummon();
OpCodeControlSequence activatePlayableSummon(AnimScriptEvent* srEvent);
void initializeIfrit(InitSummonEvent* summEvent);
void initPlayableSummons();
void loadPlayableSummonAnimScripts();
void loadPlayableSummonAnimations();
#endif // !CONTROLLABLE_SUMMON_H
