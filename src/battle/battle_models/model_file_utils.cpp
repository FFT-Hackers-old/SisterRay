#include "model_file_utils.h"
#include "../../impl.h"


void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    srLogWrite("attempting to call game LGP load with arguments LGP Context: %p, bytesBuf: %p, filename:%s", context, bytesReadBuf, filename);
    srLogWrite("lgp context: %i, %i, %i, %p", context->mode, context->useLGP, context->lgpTableIdx, context->mangler);
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

LGPArchiveFile srOpenDAFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer) {
    i32 bytesRead = 0;
    void* daArchiveFile;
    char daFileNameBuffer[204];
    char mangledName[32];
    u32 archiveIndex = 0;

    createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
    context->mangler(&(daFileNameBuffer[0]), &(mangledName[0]));

    //srLogWrite("mangled name: %s", mangledName);
    LGPArchiveFile archiveFile = lgpArchiveRead((u8*)battleLGPBuffer, mangledName);
    return archiveFile;
}

LGPArchiveFile srOpenABFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer) {
    i32 bytesRead = 0;
    void *abFile;
    char abFileNameBuffer[204];
    char mangledName[32];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    context->mangler(&(abFileNameBuffer[0]), &(mangledName[0]));

    //srLogWrite("mangled name: %s", mangledName);
    LGPArchiveFile archiveFile = lgpArchiveRead((u8*)battleLGPBuffer, mangledName);
    return archiveFile;
}
