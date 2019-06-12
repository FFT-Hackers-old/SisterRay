#include "model_file_utils.h"


void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

void* srOpenDAFile(LGPContext* context, char* baseFileName) {
    i32 bytesRead;
    void* daArchiveFile;
    char daFileNameBuffer[204];

    u32 archiveIndex = 0;
    createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
    daArchiveFile = srLoadLGPFile(context, &bytesRead, &(daFileNameBuffer[0]));// loads the da file
    return daArchiveFile;
}

void* srOpenABFile(LGPContext* context, char* baseFileName) {
    i32 bytesRead;
    void *abFile;
    char abFileNameBuffer[204];

    createABFilename(baseFileName, &(abFileNameBuffer[0]));
    abFile = srLoadLGPFile(context, &bytesRead, &(abFileNameBuffer[0]));// loads the da file
    return abFile;
}
