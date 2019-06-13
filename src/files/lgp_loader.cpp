#include "lgp_loader.h"
#include "../impl.h"


void setupLGPTable(char *filePath, u32 tableIdx) {
    u32* tablePtr = (u32*)0xDB2A98;
    srLogWrite("attempting to initialize LGP table for lgp %s, at index %i", filePath, tableIdx);
    ff7InitLGPTableIdx(filePath, tableIdx);
    srLogWrite("state of LGP index int table: %p", (tablePtr[2]));
}
