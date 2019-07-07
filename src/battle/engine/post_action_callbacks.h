#ifndef POST_ACTION_CALLBACKS_H
#define POST_ACTION_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../../events/battle_events.h"

void handleCounters(ActionContextEvent* actionEvent);
void setMimeData(ActionContextEvent* actionEvent);
void handleSense(ActionContextEvent* actionEvent);
void handleLuckySevens(ActionContextEvent* actionEvent);
void handleAddedCut(ActionContextEvent* actionEvent);
#endif 
