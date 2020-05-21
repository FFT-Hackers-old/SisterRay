#include "status_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../gamedata/base_type_names.h"

void statExitMenuListener(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    else {
        playMenuSound(4);
        sub_6C9812(5, 0);
        setActiveMenu(0);
    }
}

/*Handlers for L1/R1 "switching" inputs, for states where they function*/
void statChangeCharLeft(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    do {
        *STAT_MENU_ACTIVE_ACTOR = (((i32)(*STAT_MENU_ACTIVE_ACTOR) - 1) < 0) ? 2 : ((*STAT_MENU_ACTIVE_ACTOR) - 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*STAT_MENU_ACTIVE_ACTOR] == 0xFF);

    //update displayed character Data in the Widget
}

void statChangeCharRight(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    do {
        *STAT_MENU_ACTIVE_ACTOR = (((*STAT_MENU_ACTIVE_ACTOR) + 1) > 2) ? 0 : ((*STAT_MENU_ACTIVE_ACTOR) + 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*STAT_MENU_ACTIVE_ACTOR] == 0xFF);
}
