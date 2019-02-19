#include "string_registry.h"
#include "string.h"

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
