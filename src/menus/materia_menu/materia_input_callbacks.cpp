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
    switch (optionChoice.relativeColumnIndex) {
        case 1: {
            setMenuState(event->menu, 3);
            break;
        }
        case 2: {
            if (*byte_DC0B4B & 1) {
                playMenuSound(3);
                return;
            }
            setMenuState(event->menu, 7);
            *dword_DC1108 = 1;
            break;
        }
        default: {
            playMenuSound(1);
            setMenuState(event->menu, 1);
        }
    }
}

void enterOptionView(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;
    playMenuSound(1);
    setMenuState(event->menu, 0);
}

void enterMateriaViewHandler(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;

    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;
    u8 characterID = G_SAVE_MAP->activeParty[*EQUIP_MENU_PARTY_INDEX];
    auto& characterRecord = gContext.party.getActivePartyCharacter(characterID);
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    u8* slotsPtr = getMateriaSlots(gearSlot.equippedIdx, gearSlot.slotGearType);
 
    srLogWrite("Weapon Slots: %d, %d, %d, %d", slotsPtr[0], slotsPtr[1], slotsPtr[2], slotsPtr[3]);
    srLogWrite("CHECKING VALIDITY OF SLOT TYPE: %d", slotsPtr[activeSlot]);
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

    playMenuSound(1);
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;
    auto materiaChoice = getStateCursor(event->menu, 2)->context;

    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    auto equippedMateria = gearSlot.materia[activeSlot];
    auto toEquipMateria = gContext.materiaInventory->getResource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
    gContext.materiaInventory->updateResource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex, equippedMateria);
    gearSlot.materia[activeSlot] = toEquipMateria;
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

    auto& characterRecord = gContext.party.getActivePartyCharacter(*MAT_MENU_PARTY_INDEX);
    auto arrangeChoice = getStateCursor(event->menu, 7)->context;
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
            for (auto& gearSlot : characterRecord.equipment) {
                for (auto slotIdx = 0; slotIdx < 8; ++slotIdx) { //probably factor out into a "remove all materia" method
                    auto equippedMateria = gearSlot.materia[slotIdx];
                    if (equippedMateria.materiaID != 0xFFFF && !(gContext.materiaInventory->isFull())) {
                        gearSlot.materia[slotIdx].materiaID = 0xFFFF;
                        gearSlot.materia[slotIdx].materiaAP = 0;
                        gContext.materiaInventory->insertIntoMateriaInventory(equippedMateria);
                    }
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

    if (gContext.materiaInventory->getResource(rowPosition).materiaID == -1) {
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
    if ((event->menuState != 0))
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

    auto& characterRecord = gContext.party.getActivePartyCharacter(*MAT_MENU_PARTY_INDEX);
    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto activeSlot = slotChoice.relativeColumnIndex;

    if (gContext.materiaInventory->isFull()) {
        playMenuSound(3);
        return;
    }

    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    auto equippedMateria = gearSlot.materia[activeSlot];
    if (equippedMateria.materiaID = 0xFFFF) {
        playMenuSound(3);
        return;
    }
    gearSlot.materia[activeSlot].materiaID = 0xFFFF;
    gearSlot.materia[activeSlot].materiaAP = 0;

    playMenuSound(447);
    gContext.materiaInventory->insertIntoMateriaInventory(equippedMateria);
    srUpdatePartyMember(*MAT_MENU_PARTY_INDEX);

}
