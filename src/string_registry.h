#ifndef STRING_REGISTRY_H
#define STRING_REGISTRY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include <vector>

/*index of string starts in a buffer which is resizeable*/
typedef struct {
    std::vector<u32> string_index_map;
    int buffer_length; //current maximum buffer_length
    int insertion_point; //index to insert the next string
    char* string_buffer;
} stringRegistry;

/*Holds the games strings, via ptrs to buffers and index maps*/
typedef struct {
    stringRegistry  character_specific_strings[9];
    stringRegistry  item_descriptions;
    stringRegistry  item_names;
    stringRegistry  armor_descriptions;
    stringRegistry  armor_names;
    stringRegistry  accessory_descriptions;
    stringRegistry  accessory_names;
    stringRegistry  weapon_descriptions;
    stringRegistry  weapon_names;
} SrGameStrings;

void register_string(stringRegistry& registry, char* string);
char* get_string(stringRegistry& registry, int index);
void* encode_ffvii_string(char* ascii_string); //encode an ascii string to ffvii form in place

#endif
