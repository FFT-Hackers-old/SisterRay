#include "string_registry.h"
#include "string.h"
#include "impl.h"

const char* StringRegistry::get_string(int index)
{
    return getResource(index).str();
}

const char* StringRegistry::get_unicode_string(int index)
{
    return getResource(index).unicode();
}

void StringRegistry::set_string(int index, const char* str)
{
    updateResource(index, EncodedString(str));
}

void StringRegistry::set_unicode_string(int index, const char* str)
{
    updateResource(index, EncodedString::from_unicode(str));
}

/*Initialize all string registries for various string resources
  This change this to take a kernel2 stream and create a constructor
  overload for String Registry which inits it with the right strings*/
void initGameStrings(){
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Wpn"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Arm"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Acc"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Attack"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Hit"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Defense"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Evade"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("M.Atk"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("M.Defense"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("M.Evade"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("None"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Normal"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Double"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Triple"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Slots"));
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("Growth"));
   

    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Use"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Arrange"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Key Items"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Customize"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Field"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Battle"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Throw"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Type"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Name"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Most"));
    gContext.gameStrings.inventory_menu_texts.addResource(EncodedString::from_unicode("Least"));
    
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Check"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Arrange"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Wpn:"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Arm:"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Next Level:"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("AP:"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Ability List"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("Equip Effects"));
    gContext.gameStrings.materiaMenuTexts.addResource(EncodedString::from_unicode("MASTER"));

    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Fire"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Ice"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Thunder"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Earth"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Poison"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Gravity"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Water"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Wind"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Holy"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Life"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Slash"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Blunt"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Pierce"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Shoot"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Blast"));
    gContext.gameStrings.elementNames.addResource(EncodedString::from_unicode("Shadow"));

    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Strength"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Vitality"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Magic"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Spirit"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Dexterity"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Luck"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Attack"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Defense"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Max HP"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("Max MP"));
    gContext.gameStrings.statNames.addResource(EncodedString::from_unicode("EXP"));

    gContext.gameStrings.materiaTexts.setResource(11, EncodedString::from_unicode("Cover"));
    gContext.gameStrings.materiaTexts.setResource(83, EncodedString::from_unicode("C.Atk"));
    
}

const char* getNameFromRelativeID(u16 relativeID, u8 itemType) {
    srLogWrite("attempting to fetch string with relative ID: %i", relativeID);
    switch (itemType) {
    case 0:
        return gContext.items.getResource(relativeID).itemName.str();
        break;
    case 1:
        return gContext.gameStrings.weapon_names.get_string(relativeID);
        break;
    case 2:
        return gContext.gameStrings.armor_names.get_string(relativeID);
        break;
    case 3:
        return gContext.gameStrings.accessory_names.get_string(relativeID);
        break;
    default:
        return gContext.gameStrings.item_names.get_string(relativeID);
    }
}

const char* getDescriptionFromRelativeID(u16 relativeID, u8 itemType) {
    switch (itemType) {
    case 0:
        return gContext.items.getResource(relativeID).itemDescription.str();
        break;
    case 1:
        return gContext.gameStrings.weapon_descriptions.get_string(relativeID);
        break;
    case 2:
        return gContext.gameStrings.armor_descriptions.get_string(relativeID);
        break;
    case 3:
        return gContext.gameStrings.accessory_descriptions.get_string(relativeID);
        break;
    default:
        return gContext.gameStrings.item_descriptions.get_string(relativeID);
    }
}

const char* getNameFromItemID(u16 itemID){
    auto itemType = gContext.itemTypeData.getResource(itemID).itemType;
    auto relativeID = gContext.itemTypeData.getResource(itemID).typeRelativeID;
    return getNameFromRelativeID(relativeID, itemType);
}

const char* getDescriptionFromID(u16 itemID) {
    auto itemType = gContext.itemTypeData.getResource(itemID).itemType;
    auto relativeID = gContext.itemTypeData.getResource(itemID).typeRelativeID;
    return getDescriptionFromRelativeID(relativeID, itemType);
}


const char* getDefaultString() {
    return defaultString;
}
