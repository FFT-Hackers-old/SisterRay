#ifndef SISTERRAY_API_H
#define SISTERRAY_API_H

#include <SisterRay/SisterRay.h>
#include "gamedata/game_data_interface.h"

/*
 * This header contains the internal signatures of every
 * API function.
 * It's used to export these functions to the registry
 * for mod consumption.
 */

//Callback/Event API
SISTERRAY_API u32 srAddListener(SrEventType eventType, SrEventCallback callback, const char* modName, SrEventContext defaultContext);

//API for accessing registries

//Menu manipulation API
SISTERRAY_API Menu* getMenu(char* menuName);
SISTERRAY_API void addState(Menu* menu, CursorContext* context);
SISTERRAY_API CursorContext* getStateCursor(Menu* menu, i32 menuState);
SISTERRAY_API void setStateCursor(Menu* menu, i32 menuState, CursorContext* context);
SISTERRAY_API i32 getMenuState(Menu* menu);
SISTERRAY_API void setMenuState(Menu* menu, i32 value);
SISTERRAY_API Widget* getWidget(Menu* menu);

//Widget manipulation API
SISTERRAY_API Widget* srGetChild(Widget* parent, char* name);
SISTERRAY_API void moveWidget(Widget* widget, u32 x, u32 y);
SISTERRAY_API void enableWidget(Widget* widget);
SISTERRAY_API void disableWidget(Widget* widget);
SISTERRAY_API i32 getWidgetX(Widget* widget);
SISTERRAY_API i32 getWidgetY(Widget* widget);

SISTERRAY_API void setTextParams(drawTextParams* params, i32 xCoord, i32 yCoord, const char* text, color textColor, float priority);
SISTERRAY_API void srNewTextWidget(Widget* parent, drawTextParams params, char* name);
SISTERRAY_API void updateText(Widget* widget, const char* text);
SISTERRAY_API void updateTextColor(Widget* widget, color textColor);
SISTERRAY_API void updateTextPriority(Widget* widget, float priority);
SISTERRAY_API const char* getText(Widget* widget);
SISTERRAY_API color getTextColor(Widget* widget);
SISTERRAY_API float getTextPriority(Widget* widget);

SISTERRAY_API void setNumberParams(drawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority);
SISTERRAY_API void srNewNumberWidget(Widget* parent, drawNumberParams params, char* name);
SISTERRAY_API void updateNumber(Widget* widget, u32 number);
SISTERRAY_API void updateNumberColor(Widget* widget, color numberColor);
SISTERRAY_API void updateNumberPriority(Widget* widget, float priority);
SISTERRAY_API u32 getNumber(Widget* widget);
SISTERRAY_API color getNumberColor(Widget* widget);
SISTERRAY_API float getNumberPriority(Widget* widget);

SISTERRAY_API void srNewBoxWidget(Widget* parent, drawBoxParams params, char* name);
SISTERRAY_API void setBoxParams(drawBoxParams* params, i16 drawDistance1, i16 drawDistance2, u16 drawDistance3, u16 drawDistance4, float priority);
SISTERRAY_API void resizeBox(Widget* widget, i16 drawDistance1, i16 drawDistance2, i16 drawDistance3, i16 drawdistance4);
SISTERRAY_API void updateBoxPriority(Widget* widget, float priority);
SISTERRAY_API i16 getBoxDrawDistance(Widget* widget, i32 dimension);
SISTERRAY_API float getBoxPriority(Widget* widget);

SISTERRAY_API void srNewPortraitWidget(Widget* parent, drawPortraitParams params, char* name);
SISTERRAY_API void setPortraitParams(drawPortraitParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority);
SISTERRAY_API void updatePortraitPartyIndex(Widget* widget, u8 portraitIndex);

SISTERRAY_API void srNewHPBarWidget(Widget* parent, drawHPBarParams params, char* name);
SISTERRAY_API void setHPBarParams(drawHPBarParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority);
SISTERRAY_API void updateHPBarPartyIndex(Widget* widget, u8 portraitIndex);

SISTERRAY_API void srNewSlotsWidget(Widget* parent, drawSlotsParams params, char* name);
SISTERRAY_API void setSlotsParams(drawSlotsParams* params, i32 xCoordinate, i32 yCoordinate, u8* materiaSlotData);
SISTERRAY_API void updateMateriaSlots(Widget* widget, u8* materiaSlotsData);

SISTERRAY_API void srNewArrowWidget(Widget* parent, drawArrowParams params, char* name);
SISTERRAY_API void setArrowParams(drawArrowParams* params, i32 xCoordinate, i32 yCoordinate, u8 arrowCode, color arrowColor, float priority);

SISTERRAY_API void srNewGridWidget(Widget* parent, drawGridParams params, char* name, u16 srWidgetTypeID);

//Menu Manipulation API

#endif
