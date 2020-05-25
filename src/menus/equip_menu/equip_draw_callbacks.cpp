#include "equip_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../gamedata/stat_names.h"

using namespace EquipWidgetNames;

void handleChangeCharacter(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto topWidget = getChild(menuWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);
}

void handleUpdateEquipDescription(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    u16 kernelObjectID;
    const char * fetchedDescription;

    auto gearDescWidget = getChild(menuWidget, GEAR_DESC_WIDGET_NAME);
    auto descrptionWidget = getChild(gearDescWidget, GEAR_DESCRIPTION);


    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    if (event->menuState == 1) {
        auto gearChoice = getStateCursor(event->menu, 1)->context;
        kernelObjectID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relativeItemID;
        fetchedDescription = getGearDescriptionFromRelativeID(kernelObjectID, gearSlot.slotGearType);
    }
    else {
        fetchedDescription = gearSlot.equipped.gearDescription.str();
    }
    updateText(descrptionWidget, fetchedDescription);
}

void handleUpdateGearSlotsWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    u16 kernelObjectID;
    u8* materiaSlots;
    u8 materiaGrowth;

    auto gearSlotsWidget = getChild(menuWidget, GEAR_SLOTS_WIDGET_NAME);
    auto materiaSlotsWidget = getChild(gearSlotsWidget, GEAR_SLOTS);
    auto growthWidget = getChild(gearSlotsWidget, GEAR_GROWTH);


    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    if (event->menuState == 1) {
        auto gearChoice = getStateCursor(event->menu, 1)->context;
        kernelObjectID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relativeItemID;
    }
    else {
        kernelObjectID = gearSlot.equippedIdx;
    }

    switch (gearSlot.slotGearType) {
        case SR_GEAR_WEAPON: {
            materiaSlots = &(gContext.weapons.getResource(kernelObjectID).gameWeapon.materiaSlots[0]);
            materiaGrowth = gContext.weapons.getResource(kernelObjectID).gameWeapon.materia_growth;
            break;
        }
        case SR_GEAR_ARMOR: {
            materiaSlots = &(gContext.armors.getResource(kernelObjectID).gameArmor.materiaSlots[0]);
            materiaGrowth = gContext.armors.getResource(kernelObjectID).gameArmor.materia_growth;
            break;
        }
        default: {
            disableWidget(materiaSlotsWidget);
            disableWidget(growthWidget);
            materiaSlots = nullptr;
            materiaGrowth = 4;
            return;
        }
    }

    enableWidget(materiaSlotsWidget);
    enableWidget(growthWidget);
    updateMateriaSlots(materiaSlotsWidget, materiaSlots);

    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    const char * menuText = gContext.gameStrings.equipMenuTexts.get_string(materiaGrowth + 11);
    i32 growthTypeY = equipMenuWindowConfig[2].y + 64;
    i32 growthTypeX = sub_6F54A2((u8*)menuText);
    moveWidget(growthWidget, 243 - growthTypeX / 2, growthTypeY);
    updateText(growthWidget, menuText);
}

void handleUpdateStatMenuWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    auto statDiffGrid = getChild(getChild(menuWidget, STAT_DIFF_WIDGET_NAME), STAT_DIFF_GRID);
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    auto gearChoice = getStateCursor(event->menu, 1)->context;

    std::vector<std::string> armorStatOrder = { StatNames::ARMOR_DEFENSE, StatNames::EVADE, StatNames::ARMOR_MDEFENSE, StatNames::MEVADE };
    std::vector<std::string> weaponStatOrder = { StatNames::WEAPON_ATTACK, StatNames::WEAPON_ACCURACY, StatNames::WEAPON_MAGIC};

    const auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    const auto& equipped = gearSlot.equipped;
    Equippable toEquip;
    std::vector<std::string> statOrder;
    switch (gearSlot.slotGearType) {
    case SR_GEAR_WEAPON: {
        statOrder = weaponStatOrder;
        if (event->menuState == 1) {
            auto toEquipID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relativeItemID;
            toEquip = gContext.weapons.getResource(toEquipID).sharedBase;
        }
        break;
    }
    case SR_GEAR_ARMOR: {
        statOrder = armorStatOrder;
        if (event->menuState == 1) {
            auto toEquipID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relativeItemID;
            toEquip = gContext.armors.getResource(toEquipID).sharedBase;
        }
        break;
    }
    default: {
        for (auto idx = 0; idx < 7; idx++) {
            disableWidget(getChild(statDiffGrid, idx));
        }
        return;
    }
    }
    for (auto idx = 0; idx < 7; idx++) {
        if (idx > statOrder.size() - 1) {
            disableWidget(getChild(statDiffGrid, idx));
            continue;
        }
        enableWidget(getChild(statDiffGrid, idx));

        bool hasStat = equipped.stats.find(statOrder[idx]) != equipped.stats.end();
        if (!hasStat) {
            disableWidget(getChild(statDiffGrid, idx));
            continue;
        }
        auto diffRow = getChild(statDiffGrid, idx);
        if (event->menuState == 1) {
            auto currentValue = equipped.stats.at(statOrder[idx]).statValue;
            updateNumberColor(getChild(diffRow, "NEW"), getStatDisplayColor(currentValue, toEquip.stats.at(statOrder[idx]).statValue));
            updateNumber(getChild(diffRow, "NEW"), toEquip.stats.at(statOrder[idx]).statValue);
        }
        else {
            updateNumber(getChild(diffRow, "NEW"), equipped.stats.at(statOrder[idx]).statValue);
        }
        updateText(getChild(diffRow, "STA"), gContext.stats.getElement(statOrder[idx]).displayName.str());
        updateNumber(getChild(diffRow, "CUR"), equipped.stats.at(statOrder[idx]).statValue);
    }
}

void enableListWidget(const MenuDrawEvent* event) {
    auto widget = getChild(getChild(event->menu->menuWidget, GEAR_LIST_WIDGET_NAME), EQUIP_LIST);
    switch (event->menuState) {
        case 0:
            disableWidget(widget);
            break;
        case 1:
            enableWidget(widget);
            break;
        default: {
        }
    }
}

color getStatDisplayColor(u8 equippedStat, u8 toEquipStat) {
    if (toEquipStat == equippedStat) {
        return COLOR_WHITE;
    }
    if (toEquipStat > equippedStat) {
        return COLOR_GREEN;
    }
    if (toEquipStat < equippedStat) {
        return COLOR_RED;
    }
    return COLOR_WHITE;
}

