#include "model_file_utils.h"
#include "../../impl.h"


void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

/*Used when loading b file for summons and other models that do not need to be SR'd yet*/
void* srGameLoadABFile(LGPContext* context, char* baseFileName) {
    i32 bytesRead = 0;
    void *abFile;
    char abFileNameBuffer[204];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    return ff7LoadModelFile(context, &bytesRead, abFileNameBuffer);
}


void srGameInitAnimations(u32 lgpIndex, u32 unkint, void** srAnimsTable, LGPContext* context, char* filename) {
    ff7InitAnimations(lgpIndex, unkint, srAnimsTable, context, filename);
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
