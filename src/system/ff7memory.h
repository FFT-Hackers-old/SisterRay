#ifndef FF7_MEMORY_H
#define FF7_MEMORY_H

typedef void*(*srpfnsub65FF59)(size_t count, size_t size, const char* file, int line);
#define ff7allocateMemory  ((srpfnsub65FF59)0x65FF59)

#endif // !FF7_MEMORY_H
