#include "updaters.h"
#include "../impl.h"

void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    CursorContext* cursorContextArray = (CursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    if (self->collectionType != GridWidgetClass()) {
        srLogWrite("returning prematurely");
        return;
    }

    srLogWrite("Running updater");

    auto typedPtr = (CursorGridWidget*)self;
    auto gearType = gContext.gearViewData.getItemType();
    srLogWrite("flat Index used for update:%i", flatIndex);
    auto relativeItemID = gContext.gearViewData.get_resource(flatIndex).relative_item_id;
    const char* name = getNameFromRelativeID(relativeItemID, gearType);
    srLogWrite("fetched name %s for id: %i type %i", name, relativeItemID, gearType);
    updateText(widget, name);

}
