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

//Basic Widet structure
struct Widget_ {
    const WidgetClass* klass;
    std::string name;
    std::vector<Widget*> children; //Child widgets
    std::unordered_map<std::string, u32> children_names; //Name Index Lookup
    u32 xCoordinate;
    u32 yCoordinate;
};

Widget* createWidget(const WidgetClass* wclass, std::string name, size_t size);
void addChildWidget(Widget* widget, Widget* child_widget, std::string name);

#endif
