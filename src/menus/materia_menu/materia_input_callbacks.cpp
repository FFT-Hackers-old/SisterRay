#include "materia_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu_utils.h"
#include "../../inventories/inventory_utils.h"

#define COMMAND_ROW_COUNT 4

//MATERIA_MENU_OK_HANDLERS
void checkArrangeChoiceHandler(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    auto optionChoice = getStateCursor(event->menu, 0)->context;
    u32* dword_DC1108 = (u32*)(0xDC1108);

    playMenuSound(1);
    switch (optionChoice.relativeRowIndex) {
        case 0: {
            setMenuState(event->menu, 3);
            break;
        }
        case 1: {
            if (*byte_DC0B4B & 1) {
                playMenuSound(3);
                return;
            }
            setMenuState(event->menu, 7);
            *dword_DC1108 = 1;
            break;
        }
        default: {

        }
    }
}

void enterSlotView(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;
    playMenuSound(1);
    setMenuState(event->menu, 1);
}

void enterOptionView(const MateriaInputEvent* event) {
    if (event->menuState != 1 || (getStateCursor(event->menu, 1)->context.relativeColumnIndex != 0))
        return;
    playMenuSound(1);
    setMenuState(event->menu, 0);
}

void enterMateriaViewHandler(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;

    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;
    auto gearType = slotChoice.relativeRowIndex;
    u8 characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    u8* slotsPtr = nullptr;

    switch (gearType) {
        case 0: {
            slotsPtr = &(gContext.weapons.get_resource(getEquippedGear(characterID, 1)).materia_slots[0]);
        }
        case 1: {
            slotsPtr = &(gContext.armors.get_resource(getEquippedGear(characterID, 2)).materia_slots[0]);
        }
        default: {
            break;
        }
    }
    if (!(slotsPtr[activeSlot]) || *byte_DC0B4B) {
        playMenuSound(3);
        /*Insert special sephiroth stuff here for flash back*/
        return;
    }
    playMenuSound(1);
    setMenuState(event->menu, 2);
}

void equipMateriaHandler(const MenuInputEvent* event) {
    if (event->menuState != 2)
        return;

    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;
    auto materiaChoice = getStateCursor(event->menu, 2)->context;
    u8 characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    auto charName = getCharacterName(characterID);
    MateriaInventoryEntry equippedMateria;
    MateriaInventoryEntry toEquipMateria;

    auto gearType = slotChoice.relativeRowIndex;
    switch (gearType) {
        case 0: {
            equippedMateria = gContext.characters.get_element(charName).wpnMaterias[activeSlot];
            toEquipMateria = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
            gContext.materiaInventory->update_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex, equippedMateria);
            gContext.characters.get_element(charName).wpnMaterias[activeSlot] = toEquipMateria;
            break;
        }
        case 1: {
            equippedMateria = gContext.characters.get_element(charName).armMaterias[activeSlot];
            toEquipMateria = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
            gContext.materiaInventory->update_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex, equippedMateria);
            gContext.characters.get_element(charName).armMaterias[activeSlot] = toEquipMateria;
            break;
        }
        default: {
            break;
        }
    }
    srUpdatePartyMember(*MAT_MENU_PARTY_INDEX);

}

void selectCheckViewHandler(const MenuInputEvent* event) {
    if (event->menuState != 3)
        return;

    auto commandChoice = getStateCursor(event->menu, 3)->context;
    auto flatCommandIndex = (commandChoice.baseColumnIndex * COMMAND_ROW_COUNT) + commandChoice.relativeRowIndex;
    auto commandID = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].enabledCommandArray[flatCommandIndex].cursorCommandType;
    switch (commandID) {
        case 1:
        case 8: {
            setMenuState(event->menu, 4);
            break;
        }
        case 2:
        case 9: {
            setMenuState(event->menu, 5);
            break;
        }
        case 4: {
            setMenuState(event->menu, 6);
            break;
        }
    }
}

void arrangeChoiceHandler(const MenuInputEvent* event) {
    if (event->menuState != 7)
        return;

    auto arrangeChoice = getStateCursor(event->menu, 7)->context;
    u8 characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    auto charName = getCharacterName(characterID);
    switch (arrangeChoice.relativeRowIndex) {
        case 0:                             // transition to arrange
            playMenuSound(1);
            gContext.materiaInventory->sortInventory();
            break;
        case 1:                             // transition to exchange, this is currently INACTIVE as exchange will be implemented later
            break;
        case 2: {
            if (gContext.materiaInventory->isFull()) {
                playMenuSound(3);
                return;
            }
            for (auto slotIdx = 0; slotIdx < 8; ++slotIdx) { //probably factor out into a "remove all materia" method
                auto equippedWpnMateria = gContext.characters.get_element(charName).wpnMaterias[slotIdx];
                auto equippedArmMateria = gContext.characters.get_element(charName).armMaterias[slotIdx];
                if (equippedWpnMateria.item_id != 0xFFFF && !(gContext.materiaInventory->isFull())) {
                    gContext.characters.get_element(charName).wpnMaterias[slotIdx].item_id = -1;
                    gContext.characters.get_element(charName).wpnMaterias[slotIdx].materia_ap = 0;
                    gContext.materiaInventory->insertIntoMateriaInventory(equippedWpnMateria);
                }
                if (equippedArmMateria.item_id != 0xFF && !(gContext.materiaInventory->isFull())) {
                    gContext.characters.get_element(charName).armMaterias[slotIdx].item_id = -1;
                    gContext.characters.get_element(charName).armMaterias[slotIdx].materia_ap = 0;
                    gContext.materiaInventory->insertIntoMateriaInventory(equippedArmMateria);
                }
            }
            playMenuSound(447);
            srUpdatePartyMember(*MAT_MENU_PARTY_INDEX);
            break;
        }
        case 3:                             // transition to trash
            playMenuSound(1);
            setMenuState(event->menu, 8);
            break;
        default: {
        }
    }
}

void trashMateriaHandler(const MenuInputEvent* event) {
    if (event->menuState != 8)
        return;

    auto materiaChoice = getStateCursor(event->menu, 8)->context;
    auto rowPosition = materiaChoice.relativeRowIndex;

    if (gContext.materiaInventory->get_resource(rowPosition).item_id == -1) {
        playMenuSound(3);
    }
    else {
        playMenuSound(1);
        setMenuState(event->menu, 9);
    }
}

void confirmTrashHandler(const MenuInputEvent* event) {
    if (event->menuState != 9)
        return;

    auto materiaChoice = getStateCursor(event->menu, 8)->context;
    auto rowPosition = materiaChoice.relativeRowIndex;
    auto confirmChoice = getStateCursor(event->menu, 9)->context;

    if (confirmChoice.relativeRowIndex) {
        playMenuSound(1);
    }
    else {
        playMenuSound(1);
        gContext.materiaInventory->removeFromMateriaInventory(rowPosition);
        setMenuState(event->menu, 8);
        return;
    }
    setMenuState(event->menu, 9);
}

//Materia Menu Cancel Handlers
void exitMenuHandler(const MenuInputEvent* event) {
    if ((event->menuState != 0) && (event->menuState != 1))
        return;

    u32* dword_DC0E74 = (u32*)(0xDC0E74);

    playMenuSound(4);
    sub_6C9812(5, 0);
    setActiveMenu(0);
    *dword_DC0E74 = 0;
}

void exitMateriaView(const MenuInputEvent* event) {
    srLogWrite("Running exit materia handler");
    if ((event->menuState != 2))
        return;

    playMenuSound(4);
    setMenuState(event->menu, 1);
}

void exitCheckArrangeView(const MenuInputEvent* event) {
    srLogWrite("Running exit arrange handler");
    if ((event->menuState != 3) && (event->menuState != 7))
        return;

    srLogWrite("Exiting check or arrange menu State");
    playMenuSound(4);
    setMenuState(event->menu, 0);
}

void exitSpellCheckView(const MenuInputEvent* event) {
    srLogWrite("Running exit spell handler");
    if (event->menuState != 4 && event->menuState != 5 && event->menuState != 6)
        return;
    playMenuSound(4);
    setMenuState(event->menu, 3);
}

void exitTrashMateriaView(const MenuInputEvent* event) {
    srLogWrite("Running exit trash andler");
    if (event->menuState != 8)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 7);
}

void exitTrashConfirmView(const MenuInputEvent* event) {
    if (event->menuState != 9)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 8);
}

//TRIANGLE HANDLERS
void removeMateriaHandler(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;

    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;
    auto gearType = slotChoice.relativeRowIndex;
    u8 characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    auto charName = getCharacterName(characterID);
    u8* slotsPtr = nullptr;
    MateriaInventoryEntry equippedMateria;

    if (gContext.materiaInventory->isFull()) {
        playMenuSound(3);
        return;
    }

    switch (gearType) {
        case 0: {
            equippedMateria = gContext.characters.get_element(charName).wpnMaterias[activeSlot];
            if (equippedMateria.item_id = 0xFFFF) {
                playMenuSound(3);
                return;
            }
            gContext.characters.get_element(charName).wpnMaterias[activeSlot].item_id = -1;
            gContext.characters.get_element(charName).wpnMaterias[activeSlot].materia_ap = 0;
            break;
        }
        case 1: {
            equippedMateria = gContext.characters.get_element(charName).armMaterias[activeSlot];
            if (equippedMateria.item_id = 0xFFFF) {
                playMenuSound(3);
                return;
            }
            gContext.characters.get_element(charName).armMaterias[activeSlot].item_id = -1;
            gContext.characters.get_element(charName).armMaterias[activeSlot].materia_ap = 0;
            break;
        }
        default: {
        }
    }
    playMenuSound(447);
    gContext.materiaInventory->insertIntoMateriaInventory(equippedMateria);
    srUpdatePartyMember(*MAT_MENU_PARTY_INDEX);

}
