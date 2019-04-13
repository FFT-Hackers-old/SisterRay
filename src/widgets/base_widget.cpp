#include "base_widget.h"
#include "../impl.h"

//Create a default Widget on the heap, be sure to free this memory
Widget* createWidget(std::string name, size_t size, const WidgetClass* wclass) {
    Widget* widget = (Widget*)malloc(size);
    memset(widget, 0, size);
    widget->name = name;
    widget->klass = wclass;
    widget->enabled = true;
    new (&widget->childrenNames) std::unordered_map<std::string, u32>;
    new (&widget->children) std::vector<Widget*>;
    return widget;
}

CollectionWidget* createCollectionWidget(std::string name, const WidgetClass* collectionType, const WidgetClass* containedType) {
    CollectionWidget* widget = (CollectionWidget*)createWidget(name, sizeof(CollectionWidget), &kCollectionWidgetClass);
    widget->collectionType = collectionType;
    widget->containedKlass = containedType;
    return widget;
}

//Recursively draw a widget and all of its sub-widgets
void drawWidget(Widget* widget) {
    if (!widget->enabled)
        return;

    if (widget->klass == &kCollectionWidgetClass) {
        srLogWrite("drawing collection widget");
        auto typedPtr = (CollectionWidget *)widget;
        if (typedPtr->collectionType->draw) {
            srLogWrite("execution collection draw");
            typedPtr->collectionType->draw(widget);
        }
        return;
    }

    if (widget->klass->draw) {
        widget->klass->draw(widget);
    }
    else if (!widget->children.empty()) {
        for (auto it = begin(widget->children); it != end(widget->children); ++it) {
            drawWidget(*it);
        }
    }
}

//Frees a Widget and destroys all of its children
void destroyWidget(Widget* widget) {
    if (!widget->children.empty()) {
        for (auto child : widget->children)
            destroyWidget(child);
    }
    free(widget);
}

//Utility to add a child widget to a parent;
void addChildWidget(Widget* parent, Widget* child, std::string name) {
    if (parent->klass == &kCollectionWidgetClass) {
        auto typedPtr = (CollectionWidget*)parent;
        if (child->klass != typedPtr->containedKlass) {
            //Cannot add different types to a collection Widget
            return;
        }
    }


    //srLogWrite("adding child at: %p to parent at: %p" , child, parent);
    parent->children.push_back(child);
    parent->childrenNames[name] = parent->children.size() - 1;
    //srLogWrite("assigning to name %s index %i", name.c_str(), (int)parent->children.size() -1 );
}

Widget* getChild(Widget* parent, std::string name) {
    auto child = parent->children[parent->childrenNames[name]];
    //srLogWrite("returning child at: %p from parent at: %p", child, parent);
    //srLogWrite("Index of assigned name %s is %i", name.c_str(), (int)parent->childrenNames[name]);
    return child;
}

Widget* getChild(Widget* parent, u16 index) {
    if (parent->children.size() > index) {
        auto child = parent->children[index];
        return child;
    }
    else {
        return nullptr;
    }
}

//update the values of the named child widget
/*This function will do nothing if the type of the widget you pass is not compatible*/
void setChildWidget(Widget* parent, Widget* newChild, std::string name) {
    auto widgetToUpdate = getChild(parent, name);
    if (checkWidgetTypes(widgetToUpdate, newChild)) {
        auto index = parent->childrenNames[name];
        free(widgetToUpdate);
        parent->children[index] = newChild;
    }
    else {
        //YOU DID SOMETHING REAL BAD IF THIS EVER RUNS
    }
}

bool checkWidgetTypes(Widget* a, Widget* b) {
    return ((a->klass) == (b->klass));
}

void moveWidget(Widget * widget, u32 x, u32 y) {
    widget->xCoordinate = x;
    widget->yCoordinate = y;
}

void enableWidget(Widget* widget) {
    widget->enabled = true;
}

void disableWidget(Widget* widget) {
    widget->enabled = false;
}
