#ifndef USABLE_ITEM_HANDLERS_H
#define USABLE_ITEM_HANDLERS_H

#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "windows.h"

typedef bool(onUseCallback)(u16, u16, u16);


bool heal_party_member(u16 party_member_index, u16 item_index, u16 inventory_index);

#endif
