#include "container_table.h"

// create default objects from a type -- used to pre-allocate cursorContext objects
Widget* typeAllocate(WidgetClass* type, std::string name) {
    if (type == &kTextWidgetClass) {
        drawTextParams params = {};
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
    else if (type == &kSlotsWidgetClass) {
        drawSlotsParams params = {};
        return (Widget*)createSlotsWidget(params, name);
    }
}
