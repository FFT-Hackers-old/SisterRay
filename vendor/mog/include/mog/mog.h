#ifndef MOG_H
#define MOG_H

#include <stddef.h>
#include <stdint.h>

void    mogReplaceFunction(void* dst, void* newAddr);
void    mogReplaceSkip(void* dst, size_t len);
void    mogReplaceNop(void* dst, size_t len);
void*   mogVirtualAddress(uint32_t fixedAddr);


#endif
