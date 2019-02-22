#ifndef STRING_REGISTRY_H
#define STRING_REGISTRY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"
#include <vector>

class FF7EncodedString {
public:
    FF7EncodedString(char * ascii_string) {
        for (int i = 0; ascii_string[i] != '\0'; i++) {
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
    FF7EncodedString:encoded_string = ascii_string;
    }

    char* get_encoded_string() {
        return encoded_string;
    }

private:
    char* encoded_string;
};

/*std::vector based string registry, single buffer will make it to hard
  to change a string, which sucks for a modding framework*/
class StringRegistry : SrResourceRegistry<FF7EncodedString> {
public:
    /*Default empty constructor, add support to kernel2 stream*/
    StringRegistry(): SrResourceRegistry<FF7EncodedString>::SrResourceRegistry() {};
    char* get_string(int index);
};

/*Holds the games strings, via ptrs to buffers and index maps*/
typedef struct {
    StringRegistry  character_specific_strings[9];
    StringRegistry  inventory_menu_texts; //Stores texts displayed by the inventory menu handler
    StringRegistry  item_descriptions;
    StringRegistry  item_names;
    StringRegistry  armor_descriptions;
    StringRegistry  armor_names;
    StringRegistry  accessory_descriptions;
    StringRegistry  accessory_names;
    StringRegistry  weapon_descriptions;
    StringRegistry  weapon_names;
} SrGameStrings;

char* get_name_from_global_id(u16 item_id);
char* get_description_from_global_id(u16 item_id);
void init_game_strings();

#endif
