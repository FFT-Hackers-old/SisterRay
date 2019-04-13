#include "container_table.h"

static char* defaultString = "\x01\x00\x22\x41\x44\x00\x33\x54\x52\x49\x4e\x47\x00\x01\xFF";

// create default objects from a type -- used to pre-allocate CursorContext objects
Widget* typeAllocate(const WidgetClass* type, std::string name) {
    if (type == &kTextWidgetClass) {
        drawTextParams params = {0, 0, defaultString, COLOR_RED, 0.2f};
        return (Widget*)createTextWidget(params, name);
    }
    else if (type == &kNumberWidgetClass) {
        drawNumberParams params = {};
        return (Widget*)createNumberWidget(params, name);
    }
    else if (type == &kBoxWidgetClass) {
        drawBoxParams params = {};
        return (Widget*)createBoxWidget(params, name);
    }
    else if (type == &kArrowWidgetClass) {
        drawArrowParams params = {};
        return (Widget*)createArrowWidget(params, name);
    }
    else if (type == &kPortraitWidgetClass) {
        drawPortraitParams params = {};
        return (Widget*)createPortraitWidget(params, name);
    }
    else if (type == &kHPBarWidgetClass) {
        drawHPBarParams params = {};
        return (Widget*)createHPBarWidget(params, name);
    }
    else if (type == &kScrollerWidgetClass) {
        drawScrollerParams params = {};
        return (Widget*)createScrollerWidget(params, name);
    }
    else if (type == &kSlotsWidgetClass) { //This one has a pointer, needs defaults to avoid seg-fault
        drawSlotsParams params = {};
        return (Widget*)createSlotsWidget(params, name);
    }
}
