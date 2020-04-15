#ifndef STRING_DISPLAY_EVENT_H
#define STRING_DISPLAY_EVENT_H

#include <SisterRay/SisterRay.h>


#define FF7_DISPLAY_BATTLE_STRING ((void*)0x6D7245)

void nopBattleString(u32 a1);
void handleTopDisplayString(const MenuDrawEvent* event);
char* srGetStrFromGlobalBuffer(u16 strBufferIndex);
void handleBattleStrSubstitions(char* ret, const char* base);
u16 insertEncodedStr(char* dst, u16 baseStart, const char* toInject);
void setDisplayStringIdx(u16 stringIdx);

#endif
