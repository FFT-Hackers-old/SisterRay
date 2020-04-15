#include "base_widget.h"
#include "../impl.h"

//Create a default Widget on the heap, be sure to free this memory
Widget* createWidget(std::string name, size_t size, const WidgetClass* wclass) {
    Widget* widget = (Widget*)malloc(size);
    memset(widget, 0, size);
    new (&widget->name) std::string;
    new (&widget->children) std::vector<Widget*>;
    new (&widget->childrenNames) std::unordered_map<std::string, u32>;
    widget->name = name;
    widget->klass = wclass;
    widget->enabled = true;
    return widget;
}

CollectionWidget* createCollectionWidget(std::string name, const WidgetClass* collectionType, const WidgetClass* containedType, size_t allocSize) {
    if (allocSize == 0)
        allocSize = sizeof(CollectionWidget);
    CollectionWidget* widget = (CollectionWidget*)createWidget(name, allocSize, &kCollectionWidgetClass);
    widget->collectionType = collectionType;
    widget->containedKlass = containedType;
    return widget;
}


const WidgetClass* CollectionWidgetClass() {
    return &kCollectionWidgetClass;
}

SISTERRAY_API u8 isCollectionWidget(Widget* widget) {
    if (widget->klass != CollectionWidgetClass())
        return false;
    return true;
}

//Recursively draw a widget and all of its sub-widgets
void drawWidget(Widget* widget) {
    if (!widget->enabled)
        return;

    if (widget->klass == &kCollectionWidgetClass) {
        auto typedPtr = (CollectionWidget *)widget;
        if (typedPtr->collectionType->draw) {
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
    /*if (parent->klass == &kCollectionWidgetClass) {
        auto typedPtr = (CollectionWidget*)parent;
        if ((child->klass != typedPtr->containedKlass)) {
            //Cannot add different types to a collection Widget
            return;
        }
    }*/
    parent->children.push_back(child);
    parent->childrenNames[name] = parent->children.size() - 1;
}

Widget* getChild(Widget* parent, std::string name) {
    auto child = parent->children[parent->childrenNames[name]];
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


SISTERRAY_API Widget* srNewWidget(Widget* parent, const char* name) {
    auto widget = createWidget(name);
    addChildWidget(parent, widget, name);
    return widget;
}

SISTERRAY_API Widget* srGetChild(Widget* parent, const char* name) {
    auto widget = getChild(parent, std::string(name));
    return widget;
}


SISTERRAY_API Widget* srGetAllocatorBase(const char* name) {
    return createWidget(name);
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

/*Translate all child widgets relative to widgets own position
  It's a good idea to give the widget the coordinate of a box corner*/
SISTERRAY_API void moveWidget(Widget * widget, u32 x, u32 y) {
    i32 relativeMoveX;
    i32 relativeMoveY;
    i32 moveX;
    i32 moveY;
    if (!(widget->children.empty())) {
        for (auto it = begin(widget->children); it != end(widget->children); ++it) {
            auto child = *it;
            if (x > widget->xCoordinate) {
                relativeMoveX = x - widget->xCoordinate;
                moveX = child->xCoordinate += relativeMoveX;
            }
            else {
                relativeMoveX = widget->xCoordinate - x;
                moveX = child->xCoordinate -= relativeMoveX;
            }
            if (y > widget->yCoordinate) {
                relativeMoveY = y - widget->yCoordinate;
                moveY = child->yCoordinate += relativeMoveY;
            }
            else {
                relativeMoveY = widget->yCoordinate - y;
                moveY= child->yCoordinate -= relativeMoveY;
            }
            moveWidget(child, moveX, moveY);
        }
    }
    widget->xCoordinate = x;
    widget->yCoordinate = y;
}

SISTERRAY_API i32 getWidgetX(Widget* widget) {
    return widget->xCoordinate;
}

SISTERRAY_API i32 getWidgetY(Widget* widget) {
    return widget->yCoordinate;
}

SISTERRAY_API void enableWidget(Widget* widget) {
    widget->enabled = true;
}

SISTERRAY_API void disableWidget(Widget* widget) {
    widget->enabled = false;
}
