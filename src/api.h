#ifndef SISTERRAY_API_H
#define SISTERRAY_API_H

#include <SisterRay/SisterRay.h>

/*
 * This header contains the internal signatures of every
 * API function.
 * It's used to export these functions to the registry
 * for mod consumption.
 */

SISTERRAY_API SrEventListener srAddListener(SrEventType eventType, SrEventCallback callback);
//The public create methods require

SISTERRAY_API Widget* srGetChild(Widget* parent, char* name);
SISTERRAY_API void moveWidget(Widget* widget, u32 x, u32 y);
SISTERRAY_API void enableWidget(Widget* widget);
SISTERRAY_API void disableWidget(Widget* widget);

SISTERRAY_API void setTextParams(drawTextParams* params, i32 xCoord, i32 yCoord, char* text, color textColor, float priority);
SISTERRAY_API void srNewTextWidget(Widget* parent, drawTextParams params, char* name);

SISTERRAY_API void setNumberParams(drawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority);
SISTERRAY_API void srNewNumberWidget(Widget* parent, drawNumberParams params, char* name);

SISTERRAY_API void srNewBoxWidget(Widget* parent, drawBoxParams params, char* name);
SISTERRAY_API void setBoxParams(drawBoxParams* params, i16 drawDistance1, i16 drawDistance2, u16 drawDistance3, u16 drawDistance4, float priority);

SISTERRAY_API void srNewPortraitWidget(Widget* parent, drawPortraitParams params, char* name);
SISTERRAY_API void setPortraitParams(drawPortraitParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority);

SISTERRAY_API void srNewHPBarWidget(Widget* parent, drawHPBarParams params, char* name);
SISTERRAY_API void setHPBarParams(drawHPBarParams* params, i32 xCoordinate, i32 yCoordinate, u8 partyIndex, float priority);

SISTERRAY_API void srNewSlotsWidget(Widget* parent, drawSlotsParams params, char* name);
SISTERRAY_API void setSlotsParams(drawSlotsParams* params, i32 xCoordinate, i32 yCoordinate, u8* materiaSlotData);

SISTERRAY_API void srNewArrowWidget(Widget* parent, drawArrowParams params, char* name);
SISTERRAY_API void setArrowParams(drawArrowParams* params, i32 xCoordinate, i32 yCoordinate, u8 arrowCode, color arrowColor, float priority);

//Menu Manipulation functions
//Widget manipulation functions

#endif
