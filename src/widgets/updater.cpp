#include "updaters.h"
#include "../impl.h"

void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    if (self->collectionType != &kGridWidgetClass)
        return;

    auto typedPtr = (CursorGridWidget*)self;
    auto gearType = gContext.gearViewData.getItemType();
    auto relativeItemID = gContext.gearViewData.get_resource(flatIndex).relative_item_id;
    char* name = getNameFromRelativeID(relativeItemID, gearType);
    updateText(widget, name);

}
