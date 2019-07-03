#ifndef SCENE_GLOBALS_H
#define SCENE_GLOBALS_H

#include <SisterRay/SisterRay.h>


#define FF7_FORMATION_ENEMIES           ((FormationEnemies*)(0x9A8748));
#define FF7_FORMATION_SETUP             ((FormationSetup*)(0x9A8750));
#define FF7_FORMATION_CAMERA            ((FormationCamera*)(0x9A8764));
#define FF7_FORMATION_ACTOR_DATA        ((FormationActorDataArray*)(0x9A8794));

FormationEnemies* getFormationEnemies();
FormationSetup* getFormationSetup();
FormationCamera* getFormationCamera();
FormationActorDataArray* getFormationActorData();


#endif // !SCENE_GLOBALS_H
