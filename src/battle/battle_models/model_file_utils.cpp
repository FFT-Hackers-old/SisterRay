#include "model_file_utils.h"


void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename) {
    return ff7LoadModelFile(context, bytesReadBuf, filename);
}

void* srOpenDAFile(LGPContext* context, char* baseFileName) {
    i32 bytesRead;
    u32 *daArchiveFile;
    u32 *daFilePtr;
    char daFileNameBuffer[204];

    u32 archiveIndex = 0;
    createDAFilename(archiveIndex, baseFileName, &(daFileNameBuffer[0]));
    daArchiveFile = srLoadLGPFile(context, &bytesRead, &(daFileNameBuffer[0]));// loads the da file
    return daArchiveFile;
}
