#ifndef WIDGET_H
#define WIDGET_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include <vector>
#include <unordered_map>

typedef struct Widget_ Widget;
typedef void(*SRWIDGETDRAWPROC)(Widget*);
typedef struct {
    SRWIDGETDRAWPROC draw;
} WidgetClass;

static const WidgetClass kBaseWidgetClass = { nullptr };

//Basic Widet structure
struct Widget_ {
    const WidgetClass* klass;
    std::string name;
    std::vector<Widget*> children; //Child widgets
    std::unordered_map<std::string, u32> children_names; //Name Index Lookup
    u32 xCoordinate;
    u32 yCoordinate;
    bool enabled;
};

Widget* createWidget(std::string name, size_t size=sizeof(Widget), const WidgetClass* klass=&kBaseWidgetClass);
void addChildWidget(Widget* widget, Widget* child_widget, std::string name);
Widget* getChild(Widget* parent, std::string name);
void setChildWidget(Widget* parent, Widget* newChild, std::string name);
bool checkWidgetTypes(Widget* a, Widget* b);

void moveWidget(Widget* widget, u32 x, u32 y);
void enableWidget(Widget* widget);
void disableWidget(Widget* widget);

#endif
