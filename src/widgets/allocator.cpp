#include "allocator.h"
#include "../impl.h"

static char* widgetDefault = "\x01\x00\x22\x41\x44\x00\x33\x54\x52\x49\x4e\x47\x00\x01\xFF";

// create default objects from a type -- used to pre-allocate CursorContext objects
// passing initial params is NOT type safe, make sure you know what you are doing!
Widget* typeAllocate(const WidgetClass* type, std::string name, SRLISTALLOCPROC allocator) {
    if (allocator) {
        return allocator(name.c_str(), 0, 0); //Allocators need to only specify positions relative to anchor arguments, move calls can be used to position them
    }
    if (type == TextWidgetKlass()) {
        DrawTextParams params = { 0, 0, widgetDefault, COLOR_RED, 0.2f };
        return (Widget*)createTextWidget(params, name);
    }
    else if (type == NumberWidgetKlass()) {
        DrawNumberParams params = {0, 0, 0, 3, COLOR_WHITE, 0.2f};
        return (Widget*)createNumberWidget(params, name);
    }
    else if (type == BoxWidgetKlass()) {
        DrawBoxParams params = {};
        return (Widget*)createBoxWidget(params, name);
    }
    else if (type == SimpleAssetWidgetKlass()) {
        DrawSimpleAssetParams params = {};
        return (Widget*)createSimpleGameAssetWidget(params, name);
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
    else if (type == GameAssetWidgetKlass()) { //We want a grid here, this function has a lot of args, might make wrappers around multiple values. Need some way to initialize
        DrawGameAssetParams params = {};
        return (Widget*)createGameAssetWidget(params, name);
    }
    srLogWrite("attempted to allocated an unsupported widget without a provided allocator, FIX THIS");
    return nullptr;
}
