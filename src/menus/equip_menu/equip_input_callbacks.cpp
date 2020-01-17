#include "equip_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

void equipGearHandler(const MenuInputEvent* event) {
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto gearChoice = getStateCursor(event->menu, 1)->context;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    u8 equippedGearItemType;

    if (event->menuState != 1)
        return;

    playMenuSound(447);
    setMenuState(event->menu, 0);
    u16 equippedGearRelativeIndex = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relative_item_id;
    switch (slotChoice.relativeRowIndex) { //
        case 0: { //equip WEAPON
            equippedGearItemType = 1;
            handleEquipGear(characterRecord, equippedGearItemType, equippedGearRelativeIndex);
            break;
        }
        case 1: { //equip ARMOR
            equippedGearItemType = 2;
            handleEquipGear(characterRecord, equippedGearItemType, equippedGearRelativeIndex);
            break;
        }
        case 2: { //equip Accessory
            equippedGearItemType = 3;
            handleEquipGear(characterRecord, equippedGearItemType, equippedGearRelativeIndex);
            break;
        }
        default: {
        }
    }
    recalculateBaseStats(*EQUIP_MENU_PARTY_INDEX);
    recalculateDerivedStats(*EQUIP_MENU_PARTY_INDEX);
    updateMiscPartyStats();

}

void selectGearHandler(const MenuInputEvent* event) {
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    i32 cursorViewBound = 0;
    u16 equippableGearCount = 0;

    if (event->menuState != 0)
        return;

    equippableGearCount = setupGearMenu(slotChoice.relativeRowIndex + 1);
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
        sub_6C9812(4, 3);
        setActiveMenu(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
    else {
        playMenuSound(4);
        sub_6C9812(5, 0);
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
        sub_6C9812(4, 3);
        setActiveMenu(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
}

void handleUnequipAcc(const MenuInputEvent* event) {
    CharacterRecord& characterRecord = *getPartyActorCharacterRecord(*EQUIP_MENU_PARTY_INDEX);
    if (!(*byte_DC0B4B & 1) && *dword_DCA5C4 == 2) {
        playMenuSound(4);
        if (characterRecord.equipped_accessory != 0xFF) {
            auto removedGearRelativeIndex = characterRecord.equipped_accessory;
            auto removedGearAbsoluteIndex = gContext.itemTypeData.getAbsoluteID(3, removedGearRelativeIndex);
            gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteIndex, 1); //can only unequip
        }

        characterRecord.equipped_accessory = 0xFF;
        recalculateBaseStats(*EQUIP_MENU_PARTY_INDEX);
        recalculateDerivedStats(*EQUIP_MENU_PARTY_INDEX);
        updateMiscPartyStats();
    }
}

void handleEquipGear(SrCharacterData& characterRecord, u8 gearType, u8 equippedGearRelativeIndex) {
    u8 removedGearRelativeID;
    u16 removedGearAbsoluteID;
    u16 equippedGearAbsoluteID;

    switch (gearType) {
        case 1: {
            removedGearRelativeID = characterRecord.equippedWeapon;
            removedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, removedGearRelativeID);
            equippedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, equippedGearRelativeIndex);
            characterRecord.equippedWeapon = equippedGearRelativeIndex;
            handleMateriaUpdate(characterRecord, gearType, equippedGearRelativeIndex);
            break;
        }
        case 2: {
            removedGearRelativeID = characterRecord.equippedArmor;
            removedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, removedGearRelativeID);
            equippedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, equippedGearRelativeIndex);
            characterRecord.equippedArmor = equippedGearRelativeIndex;
            handleMateriaUpdate(characterRecord, gearType, equippedGearRelativeIndex);
            break;
        }
        case 3: {
            removedGearRelativeID = characterRecord.equippedAccessory;
            removedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, removedGearRelativeID);
            equippedGearAbsoluteID = gContext.itemTypeData.getAbsoluteID(gearType, equippedGearRelativeIndex);
            characterRecord.equippedAccessory = equippedGearRelativeIndex;
            break;
        }
        default: {
        }
    }

    *DID_MATERIA_GEAR_CHANGE = 1;
    gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteID, 1);
    gContext.inventory->decrementInventoryByItemID(equippedGearAbsoluteID, 1);
}

//Update Materia after new items are equipped
void handleMateriaUpdate(SrCharacterData& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex) {
    WeaponData newWeaponData;
    ArmorData newArmorData;
    MateriaInventoryEntry& equippedMateriaData = MateriaInventoryEntry();
    u8* materiaSlots;
    bool shouldRemove = false;
    auto characterID = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);

    for (i32 materiaSlotIndex = 0; materiaSlotIndex < 8; ++materiaSlotIndex) {
        switch (gearType) {
            case 1: {
                newWeaponData = gContext.weapons.getResource(gearRelativeIndex).gameWeapon;
                materiaSlots = &(newWeaponData.materia_slots[0]);
                equippedMateriaData = activeCharacterRecord.wpnMaterias[materiaSlotIndex];
                break;
            }
            case 2: {
                newArmorData = gContext.armors.getResource(gearRelativeIndex).gameArmor;
                materiaSlots = &(newArmorData.materia_slots[0]);
                equippedMateriaData = activeCharacterRecord.armMaterias[materiaSlotIndex];
                break;
            }
            default: {
                break;
            }
        }

        shouldRemove = (!(materiaSlots[materiaSlotIndex] && (equippedMateriaData.item_id != 0xFFFF)));
        if (shouldRemove) {
            *byte_DC1148 = 0;
            gContext.materiaInventory->insertIntoMateriaInventory(equippedMateriaData); //put any materia removed back on, needs to work with the SR materia
            *byte_DC1148 = 0;
            equippedMateriaData.item_id = 0xFFFF;
        }
    }
}

/*There are originally 32 slots for the equipment view, this interfaces with a new global. We construct
  We do this by just constructing a new object and binding it to the gContext, since this is an emphemeral struct*/
u16 setupGearMenu(u8 itemType) {
    u8 characterID = G_SAVE_MAP->activeParty[*EQUIP_MENU_PARTY_INDEX];
    u16 equippableGearCount = 0;
    gContext.gearViewData = SrGearViewData();
    for (i32 inventoryIdx = 0; inventoryIdx < gContext.inventory->currentCapacity(); inventoryIdx++) {
        auto inventoryEntry = gContext.inventory->getResource(inventoryIdx).item_id;
        if (gContext.itemTypeData.getResource(inventoryEntry).itemType != itemType) {
            continue;
        }
        if (characterCanEquipItem(characterID, inventoryEntry)) {
            GearViewData data = { gContext.itemTypeData.getResource(inventoryEntry).typeRelativeID };
            srLogWrite("initialized gear view data with id: %i", data.relative_item_id);
            gContext.gearViewData.addResource(data);
            equippableGearCount++;
        }
    }
    gContext.gearViewData.setItemType(itemType);
    gContext.gearViewData.setSlotsInUse(equippableGearCount);
    return equippableGearCount;
}
