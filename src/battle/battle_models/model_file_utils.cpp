#include "model_file_utils.h"
#include "../../impl.h"


void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    srLogWrite("attempting to call game LGP load with arguments LGP Context: %p, bytesBuf: %p, filename:%s", context, bytesReadBuf, filename);
    srLogWrite("lgp context: %i, %i, %i, %p", context->mode, context->useLGP, context->lgpTableIdx, context->mangler);
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

void* srOpenDAFile(LGPContext* context, const char* baseFileName) {
    i32 bytesRead = 0;
    void* daArchiveFile;
    char daFileNameBuffer[204];

    u32 archiveIndex = 0;
    createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
    srLogWrite("Printing name of animation file to seek: %s", &(daFileNameBuffer[0]));
    daArchiveFile = srLoadLGPFile(context, &bytesRead, &(daFileNameBuffer[0]));// loads the da file
    srLogWrite("file open complete, da archive ptr: %p bytes read: %i:", daArchiveFile, bytesRead);
    return daArchiveFile;
}

void* srOpenABFile(LGPContext* context, const char* baseFileName) {
    i32 bytesRead = 0;
    void *abFile;
    char abFileNameBuffer[204];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    abFile = srLoadLGPFile(context, &bytesRead, &(abFileNameBuffer[0]));// loads the da file
    return abFile;
}
