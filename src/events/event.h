#ifndef EVENT_H
#define EVENT_H

#include <SisterRay/types.h>


typedef enum SrEventType { NO_TYPE, DRAW_EQUIP_MENU, EQUIP_MENU_INPUT_OK, EQUIP_MENU_INPUT_CANCEL, EQUIP_MENU_INPUT_R1, EQUIP_MENU_INPUT_L1 };
typedef u32 SrEventListener;

#endif
