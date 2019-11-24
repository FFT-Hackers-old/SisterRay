#ifndef ANIMATION_SCRIPT_PARSER_H
#define ANIMATION_SCRIPT_PARSER_H

#include <SisterRay/SisterRay.h>

#endif // !ANIMATION_SCRIPT_PARSER_H

#pragma pack(push, 1)
typedef struct {
    u8 isScriptActive;
    u8 currentOpCode;
    u8 field_2;
    u8 field_3;
    u8* scriptPtr;
    u8 field_8;
    u8 field_9;
    u16 opCodeArgs[5];
    u8 field_14;
    u8 field_15;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u8 field_1C
}GameAnimationScriptContext
#pragma pack(pop)
