#include "string_registry.h"
#include "string.h"
#include "impl.h"

const char* StringRegistry::get_string(int index)
{
    return get_resource(index).str();
}

const char* StringRegistry::get_unicode_string(int index)
{
    return get_resource(index).unicode();
}

void StringRegistry::set_string(int index, const char* str)
{
    update_resource(index, EncodedString(str));
}

void StringRegistry::set_unicode_string(int index, const char* str)
{
    update_resource(index, EncodedString::from_unicode(str));
}

/*Initialize all string registries for various string resources
  This change this to take a kernel2 stream and create a constructor
  overload for String Registry which inits it with the right strings*/
void initGameStrings()
{
    for (int i = 0; i <= 11; i++) {
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Wpn"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Arm"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Acc"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Attack"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Hit"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Defense"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Evade"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("M.Atk"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("M.Defense"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("M.Evade"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("None"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Normal"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Double"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Triple"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Slots"));
        gContext.game_strings.equipMenuTexts.add_resource(EncodedString::from_unicode("Growth"));
    }

    for (int i = 0; i <= 11; i++) {
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Use"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Arrange"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Key Items"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Customize"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Field"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Battle"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Throw"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Type"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Name"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Most"));
        gContext.game_strings.inventory_menu_texts.add_resource(EncodedString::from_unicode("Least"));
    }
}

const char* getNameFromRelativeID(u16 relativeID, u8 itemType) {
    switch (itemType) {
    case 0:
        return gContext.game_strings.item_names.get_string(relativeID);
        break;
    case 1:
        return gContext.game_strings.weapon_names.get_string(relativeID);
        break;
    case 2:
        return gContext.game_strings.armor_names.get_string(relativeID);
        break;
    case 3:
        return gContext.game_strings.accessory_names.get_string(relativeID);
        break;
    default:
        return gContext.game_strings.item_names.get_string(relativeID);
    }
}

const char* getDescriptionFromRelativeID(u16 relativeID, u8 itemType) {
    switch (itemType) {
    case 0:
        return gContext.game_strings.item_descriptions.get_string(relativeID);
        break;
    case 1:
        return gContext.game_strings.weapon_descriptions.get_string(relativeID);
        break;
    case 2:
        return gContext.game_strings.armor_descriptions.get_string(relativeID);
        break;
    case 3:
        return gContext.game_strings.accessory_descriptions.get_string(relativeID);
        break;
    default:
        return gContext.game_strings.item_descriptions.get_string(relativeID);
    }
}

const char* getNameFromItemID(u16 item_id)
{
    auto itemType = gContext.itemTypeData.get_resource(item_id).item_type;
    auto relativeID = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    return getNameFromRelativeID(relativeID, itemType);
}

const char* getDescriptionFromID(u16 item_id) {
    auto itemType = gContext.itemTypeData.get_resource(item_id).item_type;
    auto relativeID = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    return getDescriptionFromRelativeID(relativeID, itemType);
}
