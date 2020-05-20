#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"
#include "game_data_interface.h"

#define KERNEL_ARMOR_COUNT 32

struct SrArmor_ {
    Equippable sharedBase;
    ArmorData gameArmor;
    AuxArmorData auxData;
};

class SrArmorRegistry : public SrNamedResourceRegistry<SrArmor, std::string> {
public:
    SrArmorRegistry(SrKernelStream* stream);
    SrArmorRegistry() : SrNamedResourceRegistry<SrArmor, std::string>() {}
};


void initializeArmorElements(SrArmor& armor, u16 relativeID);
void initializeArmorStats(SrArmor& armor);

#endif
