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

/*Grid widgets positions are automatically updated and track the position of the cursor they are initialized with*/
void drawGridWidget(CursorGridWidget* cursorGrid) {
    auto context = cursorGrid->cursor->context;
    auto size = cursorGrid->widget.widget.children.size();
    if (size < (context.viewRowBound * context.viewColumnBound)) {
        auto idx = size - 1;
        while (size < context.viewRowBound * context.viewColumnBound) {
            auto newWidget = typeAllocate(cursorGrid->widget.containedKlass, std::to_string(idx), cursorGrid->allocator);
            addChildWidget((Widget*)cursorGrid, newWidget, std::to_string(idx));
        }
    }
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
CursorGridWidget* createGridWidget(drawGridParams params, std::string name, const WidgetClass* childType) {
    CursorGridWidget* widget = (CursorGridWidget*)createCollectionWidget(name, &kGridWidgetClass, childType, sizeof(CursorGridWidget));
    widget->cursor = params.cursor;
    widget->updater = params.updater;
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;

    /*If a primitive childtype or allocator is specified, type allocate the results*/
    if (childType || params.allocator) {
        auto slotCount = (widget->cursor->context.viewRowBound) * (widget->cursor->context.viewColumnBound);
        for (u32 slot = 0; slot < slotCount; slot++) {
            auto name = std::to_string(slot);
            auto child = typeAllocate(childType, name, widget->allocator);
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

void drawStaticGridWidget(StaticGridWidget* staticGrid) {
    auto size = staticGrid->widget.widget.children.size();
    if (size < (staticGrid->rowCount* staticGrid->columnCount)) { //Try to expand the size using an allocator or the type information
        auto idx = size - 1;
        while (size < (staticGrid->rowCount * staticGrid->columnCount)) {
            auto newWidget = typeAllocate(staticGrid->widget.containedKlass, std::to_string(idx), staticGrid->allocator);
            if (newWidget) {
                addChildWidget((Widget*)staticGrid, newWidget, std::to_string(idx));
                continue;
            }
            srLogWrite("WARNING: insufficient space in grid widget for display and no automatic allocation possible!");
            break;
        }
    }
    for (auto rowIndex = 0; rowIndex < staticGrid->rowCount; ++rowIndex) {
        for (auto columnIndex = 0; columnIndex < staticGrid->columnCount; ++columnIndex) {
            u16 flatIndex = (staticGrid->columnCount) * (rowIndex)+(columnIndex); 
            auto child = getChild((Widget*)staticGrid, flatIndex);
            if (child) {
                auto elementX = (staticGrid->columnSpacing * columnIndex) + staticGrid->widget.widget.xCoordinate;
                auto elementY = (staticGrid->rowSpacing * rowIndex) + staticGrid->widget.widget.yCoordinate;
                moveWidget(child, elementX, elementY);
                if (staticGrid->updater) {
                    staticGrid->updater((CollectionWidget*)staticGrid, child, flatIndex);
                }
                drawWidget(getChild((Widget*)staticGrid, flatIndex));
            }
        }
    }
}


StaticGridWidget* createStaticGridWidget(DrawStaticGridParams params, std::string name, const WidgetClass* childType) {
    StaticGridWidget* widget = (StaticGridWidget*)createCollectionWidget(name, &kStaticGridWidgetClass, childType, sizeof(StaticGridWidget));
    srLogWrite("CREATING STATIC GRID WIDGET");
    srLogWrite("childType:%p", childType);
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;
    widget->columnCount= params.columnCount;
    widget->rowCount= params.rowCount;
    widget->updater = params.updater;
    widget->rowSpacing = params.rowSpacing;
    widget->columnSpacing = params.columnSpacing;
    srLogWrite("STATIC GRID WIDGET INITIALIZED");
    srLogWrite("xCoordinatea:%i", widget->widget.widget.xCoordinate);
    srLogWrite("yCoordinate:%i", widget->widget.widget.yCoordinate);
    srLogWrite("colCount:%i", widget->columnCount);
    srLogWrite("rowCount:%i:", widget->rowCount);
    srLogWrite("collectionType:%p", widget->widget.collectionType);
    srLogWrite("widgeKlassTYpe:%p", widget->widget.widget.klass);
    /*If a primitive childtype is specified, type allocate the results, otherwise add them to the collection manually*/
    if (childType || params.allocator) {
        auto slotCount = widget->columnCount * widget->rowCount;
        for (u32 slot = 0; slot < slotCount; slot++) {
            auto name = std::to_string(slot);
            auto child = typeAllocate(childType, name, widget->allocator);
            addChildWidget((Widget*)widget, child, name);
        }
    }
    return widget;
}


bool isStaticGridWidget(Widget* widget) {
    return (widget->klass == &kStaticGridWidgetClass);
}

void setStaticGridParams(DrawStaticGridParams* params, i32 xCoordinate, i32 yCoordinate,
    u16 columnCount, u16 rowCount, u16 columnSpacing, u16 rowSpacing,
    SRLISTUPDATERPROC updater, SRLISTALLOCPROC allocator) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->updater = updater;
    params->allocator = allocator;
    params->columnCount = columnCount;
    params->rowCount = rowCount;
    params->columnSpacing = columnSpacing;
    params->rowSpacing = rowSpacing;
}


