#include "menu.h"
#include <utility>

MenuWidgetRegistry::~MenuWidgetRegistry() {
    for (std::pair<std::string, Widget*> element : named_registry)
        destroyWidget(element.second);
}
