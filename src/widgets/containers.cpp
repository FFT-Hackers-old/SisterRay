#include <string>
#include <stdexcept>
#include "containers.h"
#include "../impl.h"
#include "../api.h"

/*The static grid class does not update based on a reference to a mutable cursor object*/

const WidgetClass* GridWidgetClass() {
    return &kGridWidgetClass;
}

SISTERRAY_API void srNewGridWidget(Widget* parent, drawGridParams params, char* name, u16 srWidgetTypeID) {
    const WidgetClass* childType = getChildTypeFromID(srWidgetTypeID);
    auto widget = createGridWidget(params, std::string(name), childType);
    addChildWidget(parent, (Widget*)widget, std::string(name));
}

const WidgetClass* getChildTypeFromID(u16 widgetTypeID) {
    switch (widgetTypeID) {
        case 0:
            return TextWidgetKlass();
        case 1:
            return NumberWidgetKlass();
        case 2:
            return BoxWidgetKlass();
        case 3:
            return SimpleAssetWidgetKlass();
        default: {
            throw std::invalid_argument("provided invalid widget type argument to createNewGridWidget");
        }
    }
}

/*Grid widgets positions are automatically updated and track the position of the cursor they are initialized with,
  For this reason, this collection Widget is not moved by moving the Widget directly, but by moving the underlying Cursor*/
void drawGridWidget(CursorGridWidget* cursorGrid) {
    auto context = cursorGrid->cursor->context;
    for (auto rowIndex = 0; rowIndex < context.viewRowBound; ++rowIndex) {
        for (auto columnIndex = 0; columnIndex <  context.viewColumnBound; ++columnIndex) {
            u16 flatIndex = (context.maxColumnBound) * (rowIndex) + (columnIndex);
            auto child = getChild((Widget*)cursorGrid, flatIndex);
            if (child) {
                auto elementX = (cursorGrid->cursor->columnSpacing * columnIndex) + cursorGrid->widget.widget.xCoordinate;
                auto elementY = (cursorGrid->cursor->rowSpacing * rowIndex) + cursorGrid->widget.widget.yCoordinate;
                moveWidget(child, elementX, elementY);
                u16 startIndex = ((context.maxColumnBound) * (context.baseRowIndex)) + (context.baseColumnIndex);
                if (cursorGrid->updater) {
                    cursorGrid->updater((CollectionWidget*)cursorGrid, child, startIndex+flatIndex);
                }
                drawWidget(getChild((Widget*)cursorGrid, flatIndex));
            }
        }
    }
}

/*Use this method to create self-managing grid widgets from a cursor context object, with a parametrized type
  Do not use your own childTypes here, use the pre-defined widget types in sister ray*/
CursorGridWidget* createGridWidget(drawGridParams params, std::string name, const WidgetClass* childType, void* initParams) {
    CursorGridWidget* widget = (CursorGridWidget*)createCollectionWidget(name, &kGridWidgetClass, childType, sizeof(CursorGridWidget));
    widget->cursor = params.cursor;
    widget->updater = params.updater;
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;

    /*If a primitive childtype is specified, type allocate the results*/
    if (childType) {
        auto slotCount = (widget->cursor->context.viewRowBound) * (widget->cursor->context.viewColumnBound);
        for (u32 slot = 0; slot < slotCount; slot++) {
            auto name = std::to_string(slot);
            auto child = typeAllocate(childType, name, initParams);
            addChildWidget((Widget*)widget, child, name);
        }
    }
    return widget;
}

bool isGridWidget(Widget* widget) {
    return (widget->klass == &kGridWidgetClass);
}


/*The static grid class does not update based on a reference to a mutable cursor object*/
const WidgetClass* StaticGridWidgetClass() {
    return &kStaticGridWidgetClass;
}

void drawStaticGridWidget(StaticGridWidget* cursorGrid) {
    for (auto rowIndex = 0; rowIndex < cursorGrid->rowCount; ++rowIndex) {
        for (auto columnIndex = 0; columnIndex < cursorGrid->columnCount; ++columnIndex) {
            u16 flatIndex = (cursorGrid->columnCount) * (rowIndex)+(columnIndex); 
            auto child = getChild((Widget*)cursorGrid, flatIndex);
            if (child) {
                auto elementX = (cursorGrid->columnSpacing * columnIndex) + cursorGrid->widget.widget.xCoordinate;
                auto elementY = (cursorGrid->rowSpacing * rowIndex) + cursorGrid->widget.widget.yCoordinate;
                moveWidget(child, elementX, elementY);
                if (cursorGrid->updater) {
                    cursorGrid->updater((CollectionWidget*)cursorGrid, child, flatIndex);
                }
                drawWidget(getChild((Widget*)cursorGrid, flatIndex));
            }
        }
    }
}


StaticGridWidget* createStaticGridWidget(DrawStaticGridParams params, std::string name, const WidgetClass* childType, void* initParams) {
    StaticGridWidget* widget = (StaticGridWidget*)createCollectionWidget(name, &kStaticGridWidgetClass, childType, sizeof(StaticGridWidget));
    widget->columnCount= params.columCount;
    widget->rowCount= params.rowCount;
    widget->updater = params.updater;
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;
    widget->rowSpacing = params.rowSpacing;
    widget->columnSpacing = params.columnSpacing;

    /*If a primitive childtype is specified, type allocate the results, otherwise add them to the collection manually*/
    if (childType) {
        auto slotCount = widget->columnCount * widget->rowCount;
        for (u32 slot = 0; slot < slotCount; slot++) {
            auto name = std::to_string(slot);
            auto child = typeAllocate(childType, name, initParams);
            addChildWidget((Widget*)widget, child, name);
        }
    }
    return widget;
}


bool isStaticGridWidget(Widget* widget) {
    return (widget->klass == &kStaticGridWidgetClass);
}


