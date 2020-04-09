#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"
#include "game_data_interface.h"

#define KERNEL_ARMOR_COUNT 32

typedef struct {
    ArmorData gameArmor;
    AuxArmorData auxData;
    EncodedString armorName;
    EncodedString armorDescription;
    EquipmentStatBoosts equipEffects;
} SrArmor;

class SrArmorRegistry : public SrNamedResourceRegistry<SrArmor, std::string> {
public:
    SrArmorRegistry(SrKernelStream* stream);
    SrArmorRegistry() : SrNamedResourceRegistry<SrArmor, std::string>() {}
};


void initializeArmorElements(SrArmor& armor, u16 relativeID);

#endif
