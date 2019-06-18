#ifndef FF7_MEMORY_H
#define FF7_MEMORY_H

#include <SisterRay/SisterRay.h>

typedef void*(*srpfnsub65FF59)(size_t count, size_t size, const char* file, u32 line);
#define ff7allocateMemory  ((srpfnsub65FF59)0x65FF59)

typedef void(*SRPFNSUB65FB40)(void*, const char* srcfile, u32 line);
#define ff7freeMemory      ((SRPFNSUB65FB40)0x65FB40)

#endif // !FF7_MEMORY_H
