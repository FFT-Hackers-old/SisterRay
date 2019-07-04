#ifndef PRE_ACTION_CALLBACKS_H
#define PRE_ACTION_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../../events/battle_events.h"

void runPreActionScripts(ActionContextEvent* actionEvent);
void initDamageContext(ActionContextEvent* actionEvent);
void setCommandData(ActionContextEvent* actionEvent);
void handleToad(ActionContextEvent* actionEvent);
void setAllFlag(ActionContextEvent* actionEvent);
void prepareMimedAction(ActionContextEvent* actionEvent);
#endif // !PRE_ACTION_CALLBACKS_H
