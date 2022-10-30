#ifndef BASE_ITEM_H
#define BASE_ITEM_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "battle_stats.h"
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
typedef struct {
    EncodedString gearName;
    EncodedString gearDescription;
    std::unordered_map<std::string, SrStaticStat> stats;
    std::unordered_set<std::string> tags;
    EquipmentStatBoosts equipEffects;
} Equippable;

// indexed by global "materiaID"
typedef struct {
    bool isEquippable;
    u8 typeDefaultIcon; //00 = normal item, 01 = sword 02= glove, etc
} ItemType;

class ItemTypeRegistry : public SrNamedResourceRegistry<ItemType, std::string> {
public:
    ItemTypeRegistry() : SrNamedResourceRegistry<ItemType, std::string>() {}
};

void initItemTypes();


// indexed by global "item_id"
typedef struct {
    std::string itemType; 
    u16 typeRelativeID;
    u8 itemIconType; //00 = normal item, 01 = sword 02= glove, etc
} SrBaseItem;

/*Holds type information about every item in the game, used for inventory index to specific data type lookups*/
class BaseItemRegistry : public SrNamedResourceRegistry<SrBaseItem, std::string> {
public:
    BaseItemRegistry() : SrNamedResourceRegistry<SrBaseItem, std::string>() {}
    void initializeAugmentedData(std::string itemType, u32 numberToInitialize);
    u16 getAbsoluteID(std::string itemType, u8 relativeIndex);
    void appendItem(const std::string& name, std::string itemType, u8 iconType); //add element is not virtual

protected:
    std::unordered_map<std::string, std::vector<u16>> reverseIndexMaps;
};

std::string getItemTypeFromGearType(SrGameGearType gearType);
u8 getKernelIconType(std::string itemType, u16 typeRelativeIndex);
const char* getItemNameFromAbsoluteIdx(u16 absoluteIdx);
const char* getItemDescFromAbsoluteIdx(u16 absoluteIdx);
StatBoost createGearBoost(SrGameGearType gearType, u16 relativeGearIdx, bool isPercent, u16 amount, bool isNegative);
SISTERRAY_API void initBaseItems();


void populatekernelStatBoosts(EquipmentStatBoosts& statBoosts, const u8* const stats, const u8* const amts, u8 count, u16 relativeGearIdx, SrGameGearType gearType);
#endif // !BASE_ITEM_H
