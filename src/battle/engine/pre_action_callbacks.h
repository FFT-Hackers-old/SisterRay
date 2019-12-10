#ifndef PRE_ACTION_CALLBACKS_H
#define PRE_ACTION_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../battle_utils.h"

void runPreActionScripts(ActionContextEvent* actionEvent);
void initDamageContext(ActionContextEvent* actionEvent);
void setPlayerCommandData(ActionContextEvent* actionEvent);
void handleToad(ActionContextEvent* actionEvent);
void setAllFlag(ActionContextEvent* actionEvent);
void prepareMimedAction(ActionContextEvent* actionEvent);
void setEnemyCommandData(ActionContextEvent* actionEvent);
#endif // !PRE_ACTION_CALLBACKS_H
