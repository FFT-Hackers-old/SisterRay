#ifndef STRING_REGISTRY_H
#define STRING_REGISTRY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"
#include "EncodedString.h"
#include <vector>
#include <string>

static const char* defaultString = "\x01\x00\x22\x41\x44\x00\x33\x54\x52\x49\x4e\x47\x00\x01\xFF";

/*std::vector based string registry, single buffer will make it to hard
  to change a string, which sucks for a modding framework*/
class StringRegistry : public SrResourceRegistry<EncodedString> {
public:
    /*Default empty constructor, add support to kernel2 stream*/
    StringRegistry() {};
    const char* get_string(int index);
    const char* get_unicode_string(int index);
    void set_string(int index, const char* str);
    void set_unicode_string(int index, const char* str);
};

/*Holds the games strings, via ptrs to buffers and index maps*/
typedef struct {
    StringRegistry  character_specific_strings[9];
    StringRegistry  inventory_menu_texts; //Stores texts displayed by the inventory menu handler
    StringRegistry  equipMenuTexts;
    StringRegistry  materiaMenuTexts;
    StringRegistry  elementNames;
    StringRegistry  statusNames;
    StringRegistry  statNames;
    StringRegistry  materiaTexts; //Non stat texts used/displayed by materia

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


const char* getGearNameFromRelativeID(u16 relativeID, SrGameGearType itemType);
const char* getNameFromRelativeID(u16 relativeID, std::string itemType);
const char* getNameFromItemID(u16 materiaID);

const char* getGearDescriptionFromRelativeID(u16 relativeID, SrGameGearType itemType);
const char* getDescriptionFromRelativeID(u16 relativeId, std::string itemType);
const char* getDescriptionFromID(u16 materiaID);
void initGameStrings();
const char* getDefaultString();

#endif
