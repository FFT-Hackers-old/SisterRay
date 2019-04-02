#ifndef MENU_H
#define MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

enum color { COLOR_GRAY, COLOR_UNKNOWN_1, COLOR_RED, COLOR_UNKNOWN_2, COLOR_UNKNOWN_3, COLOR_TEAL, COLOR_GREEN, COLOR_WHITE };
#pragma pack(push, 1)
typedef struct {
    u16 viewSize;
    u16 maxRows;
    u16 baseRow;
    u16 position1;
    u16 position2;
    u16 position3;
    u16 position4;
    float priority;
    void set(u16 viewSize, u16 maxRow, u16 baseRow, u16 position1, u16 position2, u16 position3, u16 position4, float priority) {

    }
} drawScrollerParams;
#pragma pack(pop)

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawPortraitParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 arrowCode;
    color arrowColor;
    float arrowPriority;
} drawArrowParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8* materiaSlotData;
} drawSlotsParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawHPBarParams;

// Todo: Pixel count to figure out which draw distance is which
#pragma pack(push, 1)
typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
    float boxFloat;
    void set(i16 drawDistance1Arg, i16 drawDistance2Arg, i16 drawDistance3Arg, i16 drawDistance4Arg, float priorityArg) {
        drawDistance1 = drawDistance1Arg;
        drawDistance2 = drawDistance2Arg;
        drawDistance3 = drawDistance3Arg;
        drawDistance4 = drawDistance4Arg;
        boxFloat = priorityArg;
    }
} drawBoxParams;
#pragma pack(pop)

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    char* stringToDraw;
    color textColor;
    float textThickness;
    void set(i32 xCoordinateArg, i32 yCoordinateArg, char* stringToDrawArg, color textColorArg, float priorityArg) {
        xCoordinate = xCoordinateArg;
        yCoordinate = yCoordinateArg;
        stringToDraw = stringToDrawArg;
        textColor = textColorArg;
        textThickness = priorityArg;
    }
} drawTextParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u32 numberToDraw;
    u8 charCount;
    color numberColor;
    float numberThickness;
    void set(i32 xCoordinateArg, i32 yCoordinateArg, u32 numberToDrawArg, u8 charCountArg, color numberColorArg, float priorityArg) {
        xCoordinate = xCoordinateArg;
        yCoordinate = yCoordinateArg;
        numberToDraw = numberToDrawArg;
        charCount = charCountArg;
        numberColor = numberColorArg;
        numberThickness = priorityArg;
    }
} drawNumberParams;

typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
} oldDrawBoxParams;

#define menuWindowConfig ((oldDrawBoxParams*)0x921C78)
#define equipMenuWindowConfig ((oldDrawBoxParams*)0x920AC8)

#endif
