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
void initGameStrings() {

    /*Allocate registries for the kernel2.bin stuff*/
    gContext.game_strings.item_descriptions = StringRegistry();
    gContext.game_strings.item_names = StringRegistry();
    gContext.game_strings.weapon_descriptions = StringRegistry();
    gContext.game_strings.weapon_names = StringRegistry();
    gContext.game_strings.armor_descriptions = StringRegistry();
    gContext.game_strings.armor_names = StringRegistry();
    gContext.game_strings.accessory_descriptions = StringRegistry();
    gContext.game_strings.accessory_names = StringRegistry();
    gContext.game_strings.equipMenuTexts = StringRegistry();

    /*Initialize the string registries for character specific strings*/
    for (int i = 0; i <= 9; i++) {
        gContext.game_strings.character_specific_strings[i] = StringRegistry();
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

const char* getNameFromItemID(u16 item_id) {
    auto itemType = gContext.itemTypeData.get_resource(item_id).item_type;
    auto relativeID = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    return getNameFromRelativeID(relativeID, itemType);
}

const char* getDescriptionFromID(u16 item_id) {
    auto itemType = gContext.itemTypeData.get_resource(item_id).item_type;
    auto relativeID = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    return getDescriptionFromRelativeID(relativeID, itemType);
}
