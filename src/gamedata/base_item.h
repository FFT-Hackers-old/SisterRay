#ifndef BASE_ITEM_H
#define BASE_ITEM_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../party/battle_stats.h"
#include <map>

#define ITYPE_CONSUMABLE  0
#define ITYPE_WEAPON      1
#define ITYPE_ARMOR       2
#define ITYPE_ACC         3

#define ICONTYPE_CONSUMABLE 0
#define ICONTYPE_SWORD      1
#define ICONTYPE_GLOVE      3
#define ICONTYPE_GATGUN     2
#define ICONTYPE_STAFF      5
#define ICONTYPE_CLIP       4
#define ICONTYPE_SPEAR      9
#define ICONTYPE_SHUR       6
#define ICONTYPE_PHONE      7
#define ICONTYPE_GUN        8
#define ICONTYPE_ARMOR      10
#define ICONTYPE_ACC        11

typedef std::map<std::string, std::vector<StatBoost>> EquipmentStatBoosts;


// indexed by global "item_id"
typedef struct {
    u8 itemType; //00 = normal item, 01 = weapon, 02=armor, 03=accessory
    u16 typeRelativeID;
    u8 itemIconType; //00 = normal item, 01 = sword 02= glove, etc
} ItemTypeData;

/*Holds type information about every item in the game, used for inventory index to specific data type lookups*/
class SrItemTypeRegistry : public SrNamedResourceRegistry<ItemTypeData, std::string> {
public:
    SrItemTypeRegistry() : SrNamedResourceRegistry<ItemTypeData, std::string>() {}
    void initializeAugmentedData(u8 itemType, u32 numberToInitialize);
    u16 getAbsoluteID(u8 itemType, u8 relativeIndex);
    void appendItem(const std::string& name, u8 itemType, u8 iconType); //add element is not virtual

protected:
    std::vector<i16> reverseItemRegistry;
    std::vector<i16> reverseArmorRegistry;
    std::vector<i16> reverseAccessoryRegistry;
    std::vector<i16> reverseWeaponRegistry;
};

u8 getKernelIconType(u8 itemType, u16 typeRelativeIndex);
const char* getItemNameFromAbsoluteIdx(u16 absoluteIdx);
const char* getItemDescFromAbsoluteIdx(u16 absoluteIdx);
StatBoost createGearBoost(SrGearType gearType, u16 relativeGearIdx, bool isPercent, u16 amount, bool isNegative);
SISTERRAY_API void initItemTypeData();


void populatekernelStatBoosts(EquipmentStatBoosts& statBoosts, const u8* const stats, const u8* const amts, u8 count, u16 relativeGearIdx, SrGearType gearType);
#endif // !BASE_ITEM_H
