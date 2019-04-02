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

//Utility to add a child widget to a parent;
void addChildWidget(Widget* widget, Widget* childWidget, std::string name) {
    widget->children.push_back(childWidget);
    widget->children_names[name] = widget->children.size() - 1;
}

Widget* getChild(Widget* parent, std::string name) {
    auto child = parent->children[parent->children_names[name]];
    return child;
}

//update the values of the named child widget
/*This function will do nothing if the type of the widget you pass is not compatible*/
void setChildWidget(Widget* parent, Widget* newChild, std::string name) {
    auto widgetToUpdate = getChild(parent, name);
    if (checkWidgetTypes(widgetToUpdate, newChild)) {
        free(widgetToUpdate);
        parent->children[parent] = newChild;
    }
    else {
        //YOU DID SOMETHING REAL BAD IF THIS EVER RUNS
    }
}

bool checkWidgetTypes(Widget* a, Widget* b) {
    return ((a->klass) == (b->klass));
}
