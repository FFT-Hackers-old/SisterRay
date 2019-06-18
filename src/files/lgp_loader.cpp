#include "lgp_loader.h"
#include "../impl.h"
#include <fstream>
#include <locale>

void setupLGPTable(char *filePath, u32 tableIdx) {
    u32* tablePtr = (u32*)0xDB2A98;
    srLogWrite("attempting to initialize LGP table for lgp %s, at index %i", filePath, tableIdx);
    ff7InitLGPTableIdx(filePath, tableIdx);
    srLogWrite("state of LGP index int table: %p", (tablePtr[2]));
}

void* readLGPArchive(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    srLogWrite("attempting to open file at %s", filepath);
    char* buffer = (char*)malloc(size);
    if (!file.read(buffer, size)) {
        srLogWrite("ERROR: COULD NOT READ LGP FILE INOT BUFFER");
        return nullptr;
    }
    void* blockaddr = (void*)(buffer);
    return blockaddr;
}

/*Returns a ptr to the da file data from a raw lgp file buffer*/
LGPArchiveFile lgpArchiveRead(u8* lgpBuffer, const char* mangledName) {
    LGPHeader* headerPtr = (LGPHeader*)(lgpBuffer);
    LGPTableEntry* tablePtr = (LGPTableEntry*)&(headerPtr->lookupTable);

    auto tableIdx = 0;
    u8* archiveFilePtr = nullptr;
    u32 archiveSize = 0;
    auto matchName = lowerCaseStr(std::string(mangledName));
    while (tableIdx < headerPtr->fileCount) {
        auto tableEntry = tablePtr[tableIdx];
        if (std::string(&(tableEntry.name[0])) == matchName) {
            srLogWrite("Located matching LGP file!");
            auto fileHeadStart = lgpBuffer + tableEntry.fileOffset;
            auto fileHeader = (LGPFileHeader*)(fileHeadStart);
            archiveFilePtr = &(fileHeader->rawDataStart);
            archiveSize = fileHeader->filelength;
            break;
        }
        tableIdx++;
    }
    LGPArchiveFile archiveFile = {archiveSize, archiveFilePtr};
    return archiveFile;
}


std::string lowerCaseStr(const std::string& stringToLower) {
    std::string lowerString;

    std::locale loc;
    for (u32 charIdx = 0; charIdx < stringToLower.length(); ++charIdx) {
        lowerString += std::tolower(stringToLower.at(charIdx), loc);
    }
    return lowerString;
}
