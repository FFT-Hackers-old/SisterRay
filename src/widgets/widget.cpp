#include "widget.h"

//Create a default Widget on the heap, be sure to free this memory
Widget* createWidget(std::string name, size_t size, const WidgetClass* wclass)
{
    Widget* widget = (Widget*)malloc(size);
    memset(widget, 0, size);
    widget->name = name;
    widget->klass = wclass;
    return widget;
}

//Recursively draw a widget and all of its sub-widgets
void drawWidget(Widget* widget)
{
    if (widget->klass->draw != NULL) {
        widget->klass->draw(widget);
    }
    else {
        for (auto iterator = begin(widget->children); iterator != end(widget->children); ++iterator) {
            drawWidget(*iterator);
        }
    }
}

//Utility to add a child widget to its parent
void addChildWidget(Widget* widget, Widget* child_widget, std::string name) {
    widget->children.push_back(child_widget);
    widget->children_names[name] = widget->children.size() - 1;
}
