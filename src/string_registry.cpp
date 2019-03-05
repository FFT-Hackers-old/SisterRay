#include "string_registry.h"
#include "string.h"
#include "impl.h"

char FF7EncodedString::default_string[] = "\x01\x01\x01\x00\x22\x41\x44\x00\x33\x54\x52\x49\x4e\x47\x00\x01\x01\x01\xFF";

void FF7EncodedString::encode_ascii_string(char* ascii_string) {
    int str_size = strlen(ascii_string);
    for (int i = 0; i < str_size; i++) {
        // offset lowercase ascii characters to FFVII encoding
        if (ascii_string[i] >= 'a' && ascii_string[i] <= 'z') {
            ascii_string[i] = ascii_string[i] - 0x20;
        }
        // offset uppercase ascii character to FFVII encoding starting at 0x21
        else if (ascii_string[i] >= 'A' && ascii_string[i] <= 'Z') {
            ascii_string[i] = ascii_string[i] - 0x20;
        }
        else if (ascii_string[i] == ' ') {
            ascii_string[i] = 0; //spaces are encoded as zeroes by FFVII
        }
    }
}

char* FF7EncodedString::get_encoded_string() {
    return encoded_string;
}

char* StringRegistry::get_string(int index) {
    FF7EncodedString string = StringRegistry::get_resource(index);
    return string.get_encoded_string();
}

/*Initialize all string registries for various string resources
  This change this to take a kernel2 stream and create a constructor
  overload for String Registry which inits it with the right strings*/
void init_game_strings() {

    /*Allocate registries for the kernel2.bin stuff*/
    gContext.game_strings.item_descriptions = StringRegistry();
    gContext.game_strings.item_names = StringRegistry();
    gContext.game_strings.weapon_descriptions = StringRegistry();
    gContext.game_strings.weapon_names = StringRegistry();
    gContext.game_strings.armor_descriptions = StringRegistry();
    gContext.game_strings.armor_names = StringRegistry();
    gContext.game_strings.accessory_descriptions = StringRegistry();
    gContext.game_strings.accessory_names = StringRegistry();

    /*Initialize the string registries for character specific strings*/
    for (int i = 0; i <= 9; i++) {
        gContext.game_strings.character_specific_strings[i] = StringRegistry();
    }
}

char* getNameFromItemID(u16 item_id) {
    auto item_type = gContext.item_type_data.get_resource(item_id).item_type;
    auto relative_id = gContext.item_type_data.get_resource(item_id).type_relative_id;
    switch(item_type) {
    case 0: 
        return gContext.game_strings.item_names.get_string(relative_id);
        break;
    case 1:
        return gContext.game_strings.weapon_names.get_string(relative_id);
        break; 
    case 2:
        return gContext.game_strings.armor_names.get_string(relative_id);
        break;
    case 3:
        return gContext.game_strings.accessory_names.get_string(relative_id);
        break;
    default:
        return gContext.game_strings.item_names.get_string(relative_id);
    }
}

char* get_description_from_global_id(u16 item_id) {
    auto item_type = gContext.item_type_data.get_resource(item_id).item_type;
    auto relative_id = gContext.item_type_data.get_resource(item_id).type_relative_id;
    switch (item_type) {
    case 0:
        return gContext.game_strings.item_descriptions.get_string(relative_id);
        break;
    case 1:
        return gContext.game_strings.weapon_descriptions.get_string(relative_id);
        break;
    case 2:
        return gContext.game_strings.armor_descriptions.get_string(relative_id);
        break;
    case 3:
        return gContext.game_strings.accessory_descriptions.get_string(relative_id);
        break;
    default:
        return gContext.game_strings.item_descriptions.get_string(relative_id);
    }
}
