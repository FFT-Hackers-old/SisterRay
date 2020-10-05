#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "widgets_api.h"
#include <vector>
#include <unordered_map>
#include "../system/graphics.h"

struct TransitionCtx {
    u16 transitionDuration;
    u16 elapsedFrames;
    bool enterActive;
    bool exitActive;
};

typedef void(*SRWIDGETDRAWPROC)(Widget*);
typedef bool(*SRWIDGETTRANSITIONPROC)(Widget*);
typedef struct {
    SRWIDGETDRAWPROC draw;
} WidgetClass;

static const WidgetClass kBaseWidgetClass = { nullptr };
static const WidgetClass kCollectionWidgetClass = { nullptr };
const WidgetClass* CollectionWidgetClass();

//Private Widget structure
struct Widget_ {
    const WidgetClass* klass;
    std::string name;
    std::vector<Widget*> children; //Convert this to a C-style array
    std::unordered_map<std::string, u32> childrenNames; //Name Index Lookup
    u32 xCoordinate;
    u32 yCoordinate;
    bool enabled;
    bool active; //set after transitioning is complete
    ViewPort activePort;
    ViewPort targetPort;
    TransitionCtx transitionCtx;
    SRWIDGETTRANSITIONPROC enterCallback;
    SRWIDGETTRANSITIONPROC exitCallback;
};

struct CollectionWidget_ {
    Widget widget;
    const WidgetClass* collectionType;
    const WidgetClass* containedKlass;
};


Widget* createWidget(std::string name, size_t size = sizeof(Widget), const WidgetClass* klass = &kBaseWidgetClass);
CollectionWidget* createCollectionWidget(std::string name, const WidgetClass* collectionType, const WidgetClass* containedType, size_t allocSize);
void destroyWidget(Widget* widget);

void drawWidget(Widget* widget);
void addChildWidget(Widget* parent, Widget* child, std::string name);
Widget* getChild(Widget* parent, std::string name);
Widget* getChild(Widget* parent, u16 index);
void setChildWidget(Widget* parent, Widget* newChild, std::string name);
bool checkWidgetTypes(Widget* a, Widget* b);

#endif
