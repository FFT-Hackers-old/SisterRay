#include "impl.h"

SISTERRAY_API void srPatchAddresses(void** patchList, size_t patchCount, void* src, void* dst, size_t offset)
{
    void* patchSrc = ((char*)src) + offset;
    void* patchDst = ((char*)dst) + offset;

    for (size_t i = 0; i < patchCount; ++i)
    {
        if (!mogReplaceInstuctionAddress(patchList[i], patchSrc, patchDst))
            srLogWrite("Failed to patch at 0x%08x (0x%08x -> 0x%08x)", patchList[i], patchSrc, patchDst);
    }
}
