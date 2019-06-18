#ifndef SISTERRAY_API_H
#define SISTERRAY_API_H

#include <SisterRay/SisterRay.h>
#include "gamedata/game_data_interface.h"
#include "menus/menu_interface.h"
#include "widgets/widgets_api.h"

/*
 * This header contains the internal signatures of every
 * API function.
 * It's used to export these functions to the registry
 * for mod consumption.
 */

//Callback/Event API
SISTERRAY_API u32 srAddListener(SrEventType eventType, SrEventCallback callback, const char* modName, SrEventContext defaultContext);

//API for accessing registries

//Menu Manipulation API

#endif
