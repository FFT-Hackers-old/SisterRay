#include "string_registry.h"
#include "string.h"
#include "impl.h"

/*initialize a string registry*/
void initialize_string_registry(stringRegistry& registry, int initial_buffer_size=512) {
    registry.insertion_point = 0;
    registry.string_index_map = std::vector<u32>(128);
    registry.string_buffer = malloc(&(registry.string_buffer), initial_buffer_size);
}

/*Initialize all string registries for various string resources*/
void InitGameStrings() {
    stringRegistry& registry;

    /*Allocate registries for the kernel2.bin stuff*/
    registry = gContext.game_strings.item_descriptions;
    initialize_string_registry(registry, 1024);
    registry = gContext.game_strings.item_names;
    initialize_string_registry(registry, 512);
    registry = gContext.game_strings.weapon_descriptions;
    initialize_string_registry(registry, 1024);
    registry = gContext.game_strings.weapon_names;
    initialize_string_registry(registry, 512);
    registry = gContext.game_strings.armor_descriptions;
    initialize_string_registry(registry, 1024);
    registry = gContext.game_strings.armor_names;
    initialize_string_registry(registry, 512);
    registry = gContext.game_strings.accessory_descriptions;
    initialize_string_registry(registry, 1024);
    registry = gContext.game_strings.accessory_names;
    initialize_string_registry(registry, 512);

    /*Initialize the string registries for character specific strings*/
    for (int i = 0; i <= 9; i++) {
        initialize_string_registry(gContext.game_strings.character_specific_strings[i], 256);
    }
}

//Pass this an ascii string to add an encoded FFVII string to the registry
void register_string(stringRegistry& registry, char* string) {
    auto length_to_insert = strlen(string) + 1;
    encode_ffvii_string(string); //encode the ascii string for display in ffvii
    /*Resize the buffer to register the new string*/
    if ((registry.buffer_length - registry.insertion_point) < length_to_insert) {
        registry.buffer_length = 2 * registry.buffer_length;
        realloc(&registry.string_buffer, registry.buffer_length);
    }
    
    auto insertion_point = &(registry.string_buffer[registry.insertion_point]);
    registry.string_index_map.push_back(insertion_point);

    strncpy(insertion_point, string, length_to_insert + 1);
    registry.insertion_point = registry.insertion_point + length_to_insert + 1;
}

char* get_string(stringRegistry& registry, int index) {
    auto buffer_index = registry.string_index_map[index];
    return &(registry.string_buffer[buffer_index]);
}

/*This method will be run to encode any ascii string into (non-null terminated) FFVII encoded strings
  ascii strings may be passed to the registry and will be encoded prior to storage*/
void encode_ffvii_string(char* ascii_string) {
    for (int i = 0; ascii_string[i] != '\0'; i++) {
        // offset lowercase ascii characters to FFVII encoding
        if (ascii_string[i] >= 'a' && ascii_string[i] <= 'z'){
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
