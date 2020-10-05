#include "equip_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../gamedata/base_type_names.h"

void equipGearHandler(const MenuInputEvent* event) {
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto gearChoice = getStateCursor(event->menu, 1)->context;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    u8 equippedGearItemType;

    if (event->menuState != 1)
        return;

    playMenuSound(447);
    setMenuState(event->menu, 0);
    u16 equippedGearRelativeIndex = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relativeItemID;
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    handleEquipGear(gearSlot, equippedGearRelativeIndex);
    gContext.party.recalculatePartyMember(*EQUIP_MENU_PARTY_INDEX);
    updateMiscPartyStats();

}

void selectGearHandler(const MenuInputEvent* event) {
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    u8 cursorViewBound = 0;
    u16 equippableGearCount = 0;

    if (event->menuState != 0)
        return;

    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    equippableGearCount = setupGearMenu(gearSlot);
    srLogWrite("%d equippable items for actor %d", equippableGearCount, *EQUIP_MENU_PARTY_INDEX);
    if (equippableGearCount && !(*byte_DC0B4B & 1)) {
        playMenuSound(1);
        setMenuState(event->menu, 1);
        if (equippableGearCount <= 8) //Sets the "single view" size
            cursorViewBound = equippableGearCount;
        else
            cursorViewBound = 8;
        setContextCursorData(&(getStateCursor(event->menu, 1)->context), 0, 0, 1, cursorViewBound, 0, 0, 1, equippableGearCount, 0, 0, 0, 0, 0, 1);
    }
    else {
        playMenuSound(3);
    }
}

/*Handlers for "Cancel" inputs, one per menu State*/
void exitEquipViewListener(const MenuInputEvent* event) {
    setMenuState(event->menu, 0);
}

void exitMenuListener(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    if (*DID_MATERIA_GEAR_CHANGE && (*word_DD1BC0 || *dword_DC1290)) {
        playMenuSound(1);
        gameSetMenuTransition(4, 3);
        setActiveMenu(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
    else {
        playMenuSound(4);
        gameSetMenuTransition(5, 0);
        setActiveMenu(0);
    }
}

/*Handlers for L1/R1 "switching" inputs, for states where they function*/
void changeCharLeft(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    do {
        *EQUIP_MENU_PARTY_INDEX = (((i32)(*EQUIP_MENU_PARTY_INDEX) - 1) < 0) ? 2 : ((*EQUIP_MENU_PARTY_INDEX) - 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);

    //update displayed character Data in the Widget
}

void changeCharRight(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    do {
        *EQUIP_MENU_PARTY_INDEX = (((*EQUIP_MENU_PARTY_INDEX) + 1) > 2) ? 0 : ((*EQUIP_MENU_PARTY_INDEX) + 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);

    //update displayed character Data in the Widget
}

void changeToMateriaMenu(const MenuInputEvent* event) {
    if ((*word_DD1BC0 || *dword_DC1290)) { //When switching to the materia view, square
        playMenuSound(1);
        gameSetMenuTransition(4, 3);
        setActiveMenu(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
}

void handleUnequipAcc(const MenuInputEvent* event) {
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    if (!(*byte_DC0B4B & 1) && *dword_DCA5C4 == 2) {
        playMenuSound(4);
        if (gearSlot.equippedIdx != 0xFFFF) {
            auto removedGearRelativeIndex = gearSlot.equippedIdx;
            auto removedGearAbsoluteIndex = gContext.baseItems.getAbsoluteID(ItemTypeNames::ACCESSORY_TYPE, removedGearRelativeIndex);
            gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteIndex, 1); //can only unequip
        }
        gearSlot.equippedIdx == 0xFFFF;
        gContext.party.recalculatePartyMember(*EQUIP_MENU_PARTY_INDEX);
        updateMiscPartyStats();
    }
}

void handleEquipGear(GearSlot& gearSlot, u16 equippedGearIdx) {
    Equippable equipped;
    u16 removedGearRelativeID = gearSlot.equippedIdx;
    auto itemType = getItemTypeFromGearType(gearSlot.slotGearType);
    u16 removedGearAbsoluteID = gContext.baseItems.getAbsoluteID(itemType, removedGearRelativeID);
    u16 equippedGearAbsoluteID = gContext.baseItems.getAbsoluteID(itemType, equippedGearIdx);
    switch (gearSlot.slotGearType) {
        case SR_GEAR_WEAPON: {
            equipped = gContext.weapons.getResource(equippedGearIdx).sharedBase;
            break;
        }
        case SR_GEAR_ARMOR: {
            equipped = gContext.armors.getResource(equippedGearIdx).sharedBase;
            break;
        }
        case SR_GEAR_ACCESSORY: {
            equipped = gContext.accessories.getResource(equippedGearIdx).sharedBase;
            break;
        }
        default: {
        }
    }
    gearSlot.equippedIdx = equippedGearIdx;
    gearSlot.equipped = equipped;
    handleMateriaUpdate(gearSlot, equippedGearIdx);
    *DID_MATERIA_GEAR_CHANGE = 1;
    gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteID, 1);
    gContext.inventory->decrementInventoryByItemID(equippedGearAbsoluteID, 1);
}

//Update Materia after new items are equipped
void handleMateriaUpdate(GearSlot& gearSlot, u16 equippedGearIdx) {
    WeaponData newWeaponData;
    ArmorData newArmorData;
    MateriaInventoryEntry& equippedMateriaData = MateriaInventoryEntry();
    u8* materiaSlots;
    bool shouldRemove = false;
    auto characterID = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);
    if (gearSlot.slotGearType == SR_GEAR_ACCESSORY){
        return;
    }

    for (u8 materiaSlotIndex = 0; materiaSlotIndex < 8; ++materiaSlotIndex) {
        equippedMateriaData = gearSlot.materia[materiaSlotIndex];
        switch (gearSlot.slotGearType) {
            case SR_GEAR_WEAPON: {
                newWeaponData = gContext.weapons.getResource(equippedGearIdx).gameWeapon;
                materiaSlots = &(newWeaponData.materiaSlots[0]);
                break;
            }
            case SR_GEAR_ARMOR: {
                newArmorData = gContext.armors.getResource(equippedGearIdx).gameArmor;
                materiaSlots = &(newArmorData.materiaSlots[0]);
                break;
            }
            default: {
                break;
            }
        }

        shouldRemove = (!(materiaSlots[materiaSlotIndex] && (equippedMateriaData.materiaID != 0xFFFF)));
        if (shouldRemove) {
            *byte_DC1148 = 0;
            gContext.materiaInventory->insertIntoMateriaInventory(equippedMateriaData);
            *byte_DC1148 = 0;
            equippedMateriaData.materiaID = 0xFFFF;
        }
    }
}

/*There are originally 32 slots for the equipment view, this interfaces with a new global. We construct
  We do this by just constructing a new object and binding it to the gContext, since this is an emphemeral struct*/
u16 setupGearMenu(GearSlot& gearSlot) {
    u8 characterID = G_SAVE_MAP->activeParty[*EQUIP_MENU_PARTY_INDEX];
    u16 equippableGearCount = 0;
    gContext.gearViewData = SrGearViewData();
    srLogWrite("Checking usable gear for character %d", characterID);
    std::string itemType;
    auto gearSlotType = gearSlot.slotGearType;
    switch (gearSlotType) {
    case SR_GEAR_WEAPON: {
        itemType = ItemTypeNames::WEAPON_TYPE;
        break;
    }
    case SR_GEAR_ARMOR: {
        itemType = ItemTypeNames::ARMOR_TYPE;
        break;
    }
    case SR_GEAR_ACCESSORY: {
        itemType = ItemTypeNames::ACCESSORY_TYPE;
        break;
    }
    }

    for (u16 inventoryIdx = 0; inventoryIdx < gContext.inventory->currentCapacity(); inventoryIdx++) {
        auto inventoryEntry = gContext.inventory->getResource(inventoryIdx).materiaID;
        if (gContext.baseItems.getResource(inventoryEntry).itemType != itemType) {
            continue;
        }
        if (characterCanEquipItem(characterID, inventoryEntry)) {
            GearViewData data = { gContext.baseItems.getResource(inventoryEntry).typeRelativeID };
            srLogWrite("initialized gear view data with id: %i", data.relativeItemID);
            gContext.gearViewData.addResource(data);
            equippableGearCount++;
        }
    }
    gContext.gearViewData.setItemType(gearSlot.slotGearType);
    gContext.gearViewData.setSlotsInUse(equippableGearCount);
    return equippableGearCount;
}
