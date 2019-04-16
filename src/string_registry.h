#ifndef STRING_REGISTRY_H
#define STRING_REGISTRY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"
#include "EncodedString.h"
#include <vector>

class FF7EncodedString {
public:
    FF7EncodedString(char * ascii_string) {
        encode_ascii_string(ascii_string);
        FF7EncodedString::encoded_string = ascii_string;
    }

    FF7EncodedString(char* encoded_string, bool null_terminated) {
        if (null_terminated) {
            FF7EncodedString::encoded_string = encoded_string;
        }
    }

    FF7EncodedString() {
        encoded_string = &(FF7EncodedString::default_string[0]);
    }

    void encode_ascii_string(char* ascii_string);
    char* get_encoded_string(); 
    static char default_string[];

private:
    char* encoded_string;
};

/*std::vector based string registry, single buffer will make it to hard
  to change a string, which sucks for a modding framework*/
class StringRegistry : public SrResourceRegistry<EncodedString> {
public:
    /*Default empty constructor, add support to kernel2 stream*/
    StringRegistry() {};
    char* get_string(int index);
};

/*Holds the games strings, via ptrs to buffers and index maps*/
typedef struct {
    StringRegistry  character_specific_strings[9];
    StringRegistry  inventory_menu_texts; //Stores texts displayed by the inventory menu handler
    StringRegistry  equipMenuTexts;

    /* Kernel2 stuff */
    StringRegistry  command_descriptions;
    StringRegistry  magic_descriptions;
    StringRegistry  item_descriptions;
    StringRegistry  weapon_descriptions;
    StringRegistry  armor_descriptions;
    StringRegistry  accessory_descriptions;
    StringRegistry  materia_descriptions;
    StringRegistry  key_item_descriptions;
    StringRegistry  command_names;
    StringRegistry  magic_names;
    StringRegistry  item_names;
    StringRegistry  weapon_names;
    StringRegistry  armor_names;
    StringRegistry  accessory_names;
    StringRegistry  materia_names;
    StringRegistry  key_item_names;
    StringRegistry  battle_texts;
    StringRegistry  summon_attack_names;
} SrGameStrings;

char* getNameFromRelativeID(u16 relativeID, u8 itemType);
char* getNameFromItemID(u16 item_id);
char* getDescriptionFromRelativeID(u16 relativeId, u8 itemType);
char* getDescriptionFromID(u16 item_id);
void initGameStrings();

#endif
