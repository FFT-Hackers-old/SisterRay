#ifndef WIDGET_API_H
#define WIDGET_API_H

#include <SisterRay/SisterRay.h>

//Widget manipulation API
SISTERRAY_API Widget* srGetChild(Widget* parent, char* name);
SISTERRAY_API void moveWidget(Widget* widget, u32 x, u32 y);
SISTERRAY_API void enableWidget(Widget* widget);
SISTERRAY_API void disableWidget(Widget* widget);
SISTERRAY_API i32 getWidgetX(Widget* widget);
SISTERRAY_API i32 getWidgetY(Widget* widget);

SISTERRAY_API void setTextParams(DrawTextParams* params, i32 xCoord, i32 yCoord, const char* text, color textColor, float priority);
SISTERRAY_API void srNewTextWidget(Widget* parent, DrawTextParams params, char* name);
SISTERRAY_API void updateText(Widget* widget, const char* text);
SISTERRAY_API void updateTextColor(Widget* widget, color textColor);
SISTERRAY_API void updateTextPriority(Widget* widget, float priority);
SISTERRAY_API const char* getText(Widget* widget);
SISTERRAY_API color getTextColor(Widget* widget);
SISTERRAY_API float getTextPriority(Widget* widget);

SISTERRAY_API void setNumberParams(DrawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority);
SISTERRAY_API void srNewNumberWidget(Widget* parent, DrawNumberParams params, char* name);
SISTERRAY_API void updateNumber(Widget* widget, u32 number);
SISTERRAY_API void updateNumberColor(Widget* widget, color numberColor);
SISTERRAY_API void updateNumberPriority(Widget* widget, float priority);
SISTERRAY_API u32 getNumber(Widget* widget);
SISTERRAY_API color getNumberColor(Widget* widget);
SISTERRAY_API float getNumberPriority(Widget* widget);

SISTERRAY_API void srNewBoxWidget(Widget* parent, DrawBoxParams params, char* name);
SISTERRAY_API void setBoxParams(DrawBoxParams* params, i16 drawDistance1, i16 drawDistance2, u16 drawDistance3, u16 drawDistance4, float priority);
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
SISTERRAY_API void updateMateriaData(Widget* widget, MateriaInventoryEntry* materiaData);

SISTERRAY_API void srNewSimpleAssetWidget(Widget* parent, DrawSimpleAssetParams params, char* name);
SISTERRAY_API void setSimpleAssetParams(DrawSimpleAssetParams* params, i32 xCoordinate, i32 yCoordinate, u8 arrowCode, color arrowColor, float priority);

SISTERRAY_API void srNewGridWidget(Widget* parent, drawGridParams params, char* name, u16 srWidgetTypeID);

SISTERRAY_API void setStaticGridParams(DrawStaticGridParams* params, i32 xCoordinate, i32 yCoordinate,
    u16 columnCount, u16 rowCount, u16 columnSpacing, u16 rowSpacing,
    SRLISTUPDATERPROC updater, SRLISTALLOCPROC allocator);

#endif
