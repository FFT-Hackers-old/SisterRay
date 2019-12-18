#include "items.h"
#include "../impl.h"

SrItemRegistry::SrItemRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrItem, std::string>() {
    size_t read_size;
    SrItem item;
    ItemData baseItem;
    std::string srAttackID;

    auto idx = 0;
    auto& cmd = gContext.commands.getResource(CMD_ITEM);
    const auto attackCount = gContext.attacks.resourceCount();
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseItem, sizeof(baseItem));
        if (read_size != sizeof(baseItem))
            break;
        item.gameItem = baseItem;
        item.itemName = gContext.gameStrings.item_names.get_string(idx);
        item.itemDescription = gContext.gameStrings.item_descriptions.get_string(idx);
        SrAttack associatedAttack = attackFromItem(item);
        gContext.attacks.addElement(assembleGDataKey(attackCount + idx), associatedAttack);
        auto attackIdx = gContext.attacks.getResourceIndex(assembleGDataKey(attackCount + idx));
        item.attackIdx = attackIdx;
        cmd.commandActions.push_back(attackIdx);
        cmd.actionCount++;
        addElement(assembleGDataKey(idx), item);
        ++idx;
    }
}

SISTERRAY_API SrConsumable getSrItem(u16 modItemID, const char* modName) {
    SrConsumable srItem = SrConsumable();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& item = gContext.items.getElement(name);
    srItem.baseData = item.gameItem;
    srItem.useData = item.auxData;
    srItem.itemName = item.itemName.unicode();
    srItem.itemDesc = item.itemDescription.unicode();

    return srItem;
}

SISTERRAY_API void setSrItemData(SrConsumable data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& item = gContext.items.getElement(name);
    item.gameItem = data.baseData;
    item.auxData = data.useData;
    item.itemName = EncodedString::from_unicode(data.itemName);
    item.itemName = EncodedString::from_unicode(data.itemName);
}

SISTERRAY_API void addSrItem(SrConsumable data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto item = SrItem();
    item.gameItem = data.baseData;
    item.auxData = data.useData;
    item.itemName = EncodedString::from_unicode(data.itemName);
    item.itemName = EncodedString::from_unicode(data.itemName);
    gContext.itemTypeData.appendItem(name, ITYPE_CONSUMABLE, ICONTYPE_CONSUMABLE);

    auto& cmd = gContext.commands.getResource(CMD_ITEM);
    SrAttack associatedAttack = attackFromItem(item);
    gContext.attacks.addElement(name, associatedAttack);
    const auto attackIdx = gContext.attacks.getResourceIndex(name);
    item.attackIdx = attackIdx;
    cmd.commandActions.push_back(attackIdx);
    cmd.actionCount++;
}

SrAttack attackFromItem(const SrItem& item) {
    auto attack = SrAttack();
    attack.attackData.animationEffectID = item.gameItem.animationEffectID;
    attack.attackData.targetingFlags = item.gameItem.targetingFlags;
    attack.attackData.statusInflictType = item.gameItem.statusInflictType;
    attack.attackData.elementMask = item.gameItem.elementMask;
    attack.attackData.statusMask = item.gameItem.statusMask;
    attack.attackData.cameraMovementSingle = item.gameItem.cameraMovement;
    attack.attackData.damageFormula = item.gameItem.damageFormula;
    attack.attackData.attackPower = item.gameItem.itemPower;
    attack.attackData.specialAttackFlags = item.gameItem.specialAttackFlags;
    attack.attackData.restoreTypes = item.gameItem.restoreTypes;
    attack.attackData.additionalEffect = item.gameItem.additionalEffectID;
    attack.attackData.additionalEffectModifier = item.gameItem.additionalEffectModifier;

    attack.attackName = item.itemName;
    attack.animScriptIndex = 0xFFFF;
    attack.animationType = ITEM;
    attack.attackDescription = item.itemDescription;
    return attack;
}

/*initialize on use data to match the vanilla game*/
ConsumableUseData getOnUseData(u16 item_id) {
    switch (item_id) {
    case 0:
        return createOnUseItemData(100, 0, 0, 0, 0, 0, false, false, true);
        break;
    case 1:
        return createOnUseItemData(500, 0, 0, 0, 0, 0, false, false, true);
        break;
    case 2:
        return createOnUseItemData(10000, 0, 0, 0, 0, 0, false, false, true);
        break;
    case 3:
        return createOnUseItemData(0, 100, 0, 0, 0, 0, false, false, true);
        break;
    case 4:
        return createOnUseItemData(0, 10000, 0, 0, 0, 0, false, false, true);
        break;
    case 5:
        return createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, false, true);
        break;
    case 6:
        return createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, true, true);
        break;
    case 7:
        return createOnUseItemData(0, 0, 0, 0, 25, 0, true, false, true);
        break;
    default:
        return createOnUseItemData(0, 0, 0, 0, 0, 0, false, false, true);
    }
}

ConsumableUseData createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount,
    u16 stat_to_boost, u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all, bool requires_target) {

    ConsumableUseData consumableData = {
        hp_heal_amount,
        mp_heal_amount,
        stat_to_boost,
        character_restriction_mask,
        hp_heal_percent,
        mp_heal_percent,
        can_revive,
        target_all,
        requires_target
    };
    return consumableData;
}

SISTERRAY_API void initItems(SrKernelStream* stream) {
    gContext.items = SrItemRegistry(stream);
    gContext.itemTypeData.initializeAugmentedData((u8)0, gContext.items.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.resourceCount());
}

/*Utility check if an item is usable globally*/
bool canCharacterUseItem(u8 characterID, u16 itemID) {
    u8 itemType = gContext.itemTypeData.getResource(itemID).itemType;
    u16 relative_id = gContext.itemTypeData.getResource(itemID).itemType;
    u16 restriction_mask;
    switch(itemType) {
    case 0: {
        restriction_mask = gContext.items.getResource(itemID).auxData.characterRestrictionMask;
        if (restriction_mask & (1 << (characterID))) {
            return true;
        }
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.getResource(relative_id).equip_mask;
        if (restriction_mask & (1 << (characterID))) {
            return true;
        }
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.getResource(relative_id).equip_mask;
        if (restriction_mask & (1 << (characterID))) {
            return true;
        }
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.getResource(relative_id).equip_mask;
        if (restriction_mask & (1 << (characterID))) {
            return true;
        }
        break;
    }
    default:
        return false;
    }

    return false;
}
