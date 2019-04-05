#include "containers.h"

/*Grid widgets positions are automatically updated*/
static void drawGridWidget(CursorGridWidget* cursorGrid) {
    for (auto rowIndex = 0; rowIndex < cursorGrid->cursorContext->viewRowBound; ++rowIndex) {
        for (auto columnIndex = 0; columnIndex < cursorGrid->cursorContext->viewColumnBound; ++columnIndex) {
            u16 flatIndex = (cursorGrid->cursorContext->maxColumnBound) * (rowIndex + 1) + (columnIndex + 1);
            auto child = getChild((Widget*)cursorGrid, flatIndex);
            if (child) {
                auto elementX = cursorGrid->columnSpacing * columnIndex + cursorGrid->widget.widget.xCoordinate;
                auto elementY = cursorGrid->rowSpacing * rowIndex + cursorGrid->widget.widget.yCoordinate;
                moveWidget(child, elementX, elementY);
                cursorGrid->updater((CollectionWidget*)cursorGrid, child, flatIndex);
                drawWidget(getChild((Widget*)cursorGrid, flatIndex));
            }
        }
    }
}

CursorGridWidget* createGridWidget(GridWidgetParams params, std::string name, WidgetClass* childType) {
    CursorGridWidget* widget = (CursorGridWidget*)createCollectionWidget(name, &kGridWidgetClass, childType);
    widget->widget.widget.xCoordinate = params.xCoordinate;
    widget->widget.widget.yCoordinate = params.yCoordinate;
    widget->cursorContext = params.cursorContext;
    widget->rowSpacing = params.rowSpacing;
    widget->columnSpacing = params.columnSpacing;
    return widget;
}

bool isGridWidget(Widget* widget) {
    return (widget->klass == &kGridWidgetClass);
}
