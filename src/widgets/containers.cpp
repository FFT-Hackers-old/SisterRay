#include "containers.h"
#include <string>
#include "../impl.h"

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
                if (cursorGrid->updater) {
                    cursorGrid->updater((CollectionWidget*)cursorGrid, child, flatIndex);
                }
                drawWidget(getChild((Widget*)cursorGrid, flatIndex));
            }
        }
    }
}

/*Use this method to create self-managing grid widgets from a cursor context object, with a parametrized type
  Do not use your own childTypes here, use the pre-defined widget types in sister ray*/
CursorGridWidget* createGridWidget(GridWidgetParams params, std::string name, const WidgetClass* childType) {
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


