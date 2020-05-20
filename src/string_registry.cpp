#include "string_registry.h"
#include "string.h"
#include "impl.h"
#include "gamedata/base_type_names.h"

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
    gContext.gameStrings.equipMenuTexts.addResource(EncodedString::from_unicode("defense"));
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


const char* getGearNameFromRelativeID(u16 relativeID, SrGameGearType itemType) {
    switch (itemType) {
    case SR_GEAR_WEAPON: {
        return gContext.weapons.getResource(relativeID).sharedBase.gearName.str();
        break;
    }
    case SR_GEAR_ARMOR: {
        return gContext.armors.getResource(relativeID).sharedBase.gearName.str();
        break;
    }
    case SR_GEAR_ACCESSORY: {
        return gContext.accessories.getResource(relativeID).sharedBase.gearName.str();
        break;
    }
    default: {
        return defaultString;
        break;
    }
    }
}

const char* getNameFromRelativeID(u16 relativeID, std::string itemType) {
    srLogWrite("attempting to fetch string with relative ID: %i", relativeID);
  
    if (itemType == ItemTypeNames::CONSUMABLE_TYPE) {
        return gContext.items.getResource(relativeID).itemName.str();
    }
    if (itemType == ItemTypeNames::WEAPON_TYPE) {
        return gContext.weapons.getResource(relativeID).sharedBase.gearName.str();
    }
    if (itemType == ItemTypeNames::ARMOR_TYPE) {
        return gContext.armors.getResource(relativeID).sharedBase.gearName.str();
    }
    if (itemType == ItemTypeNames::ACCESSORY_TYPE) {
        return gContext.accessories.getResource(relativeID).sharedBase.gearName.str();
    }
    return defaultString;
}


const char* getGearDescriptionFromRelativeID(u16 relativeID, SrGameGearType itemType) {
    switch (itemType) {
    case SR_GEAR_WEAPON: {
        return gContext.weapons.getResource(relativeID).sharedBase.gearDescription.str();
        break;
    }
    case SR_GEAR_ARMOR: {
        return gContext.armors.getResource(relativeID).sharedBase.gearDescription.str();
        break;
    }
    case SR_GEAR_ACCESSORY: {
        return gContext.accessories.getResource(relativeID).sharedBase.gearDescription.str();
        break;
    }
    default: {
        return defaultString;
        break;
    }
    }
}

const char* getDescriptionFromRelativeID(u16 relativeID, std::string itemType) {

    if (itemType == ItemTypeNames::CONSUMABLE_TYPE) {
        return gContext.items.getResource(relativeID).itemDescription.str();
    }
    if (itemType == ItemTypeNames::WEAPON_TYPE) {
        return gContext.weapons.getResource(relativeID).sharedBase.gearDescription.str();
    }
    if (itemType == ItemTypeNames::ARMOR_TYPE) {
        return gContext.armors.getResource(relativeID).sharedBase.gearDescription.str();
    }
    if (itemType == ItemTypeNames::ACCESSORY_TYPE) {
        return gContext.accessories.getResource(relativeID).sharedBase.gearDescription.str();
    }
    return defaultString;
}

const char* getNameFromItemID(u16 itemID){
    auto itemType = gContext.baseItems.getResource(itemID).itemType;
    auto relativeID = gContext.baseItems.getResource(itemID).typeRelativeID;
    return getNameFromRelativeID(relativeID, itemType);
}

const char* getDescriptionFromID(u16 itemID) {
    auto itemType = gContext.baseItems.getResource(itemID).itemType;
    auto relativeID = gContext.baseItems.getResource(itemID).typeRelativeID;
    return getDescriptionFromRelativeID(relativeID, itemType);
}


const char* getDefaultString() {
    return defaultString;
}
