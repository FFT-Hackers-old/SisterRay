#include <string>
#include <stdexcept>
#include "containers.h"
#include "../impl.h"
#include "../api.h"


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
            return ArrowWidgetKlass();
        default: {
            throw std::invalid_argument("provided invalid widget type argument to createNewGridWidget");
        }
    }
}

/*Grid widgets positions are automatically updated*/
void drawGridWidget(CursorGridWidget* cursorGrid) {
    for (auto rowIndex = 0; rowIndex < cursorGrid->cursor->viewRowBound; ++rowIndex) {
        for (auto columnIndex = 0; columnIndex < cursorGrid->cursor->viewColumnBound; ++columnIndex) {
            u16 flatIndex = (cursorGrid->cursor->maxColumnBound) * (rowIndex) + (columnIndex); //Fix this math
            auto child = getChild((Widget*)cursorGrid, flatIndex);
            if (child) {
                auto elementX = cursorGrid->columnSpacing * columnIndex + cursorGrid->widget.widget.xCoordinate;
                auto elementY = cursorGrid->rowSpacing * rowIndex + cursorGrid->widget.widget.yCoordinate;
                moveWidget(child, elementX, elementY);
                srLogWrite("address of function updater: %p ", cursorGrid->updater);
                u16 startIndex = ((cursorGrid->cursor->maxColumnBound) * (cursorGrid->cursor->baseRowIndex)) + (cursorGrid->cursor->baseColumnIndex);
                if (cursorGrid->updater) {
                    srLogWrite("running update");
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
    CursorGridWidget* widget = (CursorGridWidget*)createCollectionWidget(name, &kGridWidgetClass, childType);
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;
    widget->cursor = params.cursor;
    widget->rowSpacing = params.rowSpacing;
    widget->columnSpacing = params.columnSpacing;
    widget->updater = params.updater;

    auto slotCount = (widget->cursor->viewRowBound) * (widget->cursor->viewColumnBound);
    for (u32 slot = 0; slot < slotCount; slot++) {
        auto name = std::to_string(slot);
        auto child = typeAllocate(childType, name);
        addChildWidget((Widget*)widget, child, name);
    }
    return widget;
}

bool isGridWidget(Widget* widget) {
    return (widget->klass == &kGridWidgetClass);
}


