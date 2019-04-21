#include "allocator.h"
#include "../impl.h"

static char* defaultString = "\x01\x00\x22\x41\x44\x00\x33\x54\x52\x49\x4e\x47\x00\x01\xFF";

// create default objects from a type -- used to pre-allocate CursorContext objects
Widget* typeAllocate(const WidgetClass* type, std::string name) {
    if (type == TextWidgetKlass()) {
        drawTextParams params = {0, 0, defaultString, COLOR_RED, 0.2f};
        return (Widget*)createTextWidget(params, name);
    }
    else if (type == NumberWidgetKlass()) {
        drawNumberParams params = {0, 0, 0, 3, COLOR_WHITE, 0.2f};
        return (Widget*)createNumberWidget(params, name);
    }
    else if (type == BoxWidgetKlass()) {
        drawBoxParams params = {};
        return (Widget*)createBoxWidget(params, name);
    }
    else if (type == ArrowWidgetKlass()) {
        drawArrowParams params = {};
        return (Widget*)createArrowWidget(params, name);
    }
    else if (type == PortraitWidgetKlass()) {
        drawPortraitParams params = {};
        return (Widget*)createPortraitWidget(params, name);
    }
    else if (type == HPBarWidgetKlass()) {
        drawHPBarParams params = {};
        return (Widget*)createHPBarWidget(params, name);
    }
    else if (type == ScrollerWidgetKlass()) {
        drawScrollerParams params = {};
        return (Widget*)createScrollerWidget(params, name);
    }
    else if (type == SlotsWidgetKlass()) { //This one has a pointer, needs defaults to avoid seg-fault
        drawSlotsParams params = {};
        return (Widget*)createSlotsWidget(params, name);
    }
    return nullptr;
}
