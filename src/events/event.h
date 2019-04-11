#ifndef EVENT_H
#define EVENT_H

#include <SisterRay/types.h>


typedef enum {
    NO_TYPE,
    INIT_EQUIP_MENU,
    DRAW_EQUIP_MENU,
    EQUIP_MENU_INPUT_OK,
    EQUIP_MENU_INPUT_CANCEL,
    EQUIP_MENU_INPUT_SQUARE,
    EQUIP_MENU_INPUT_TRIANGLE,
    EQUIP_MENU_INPUT_R1,
    EQUIP_MENU_INPUT_L1
} SrEventType;

typedef u32 SrEventListener;

#endif
