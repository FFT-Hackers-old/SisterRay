#ifndef USABLE_ITEM_HANDLERS_H
#define USABLE_ITEM_HANDLERS_H

#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "windows.h"
#include "inventory.h"

void heal_party_member(u8 party_member_index, u32 heal_amount, u16 inventory_index);

#endif
