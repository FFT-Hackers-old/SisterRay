#ifndef LGP_LOADER_H
#define LGP_LOADER_H

#include <SisterRay/SisterRay.h>
#include <string>

//Sets up the LGP load path, so that we may load at SR init time from the archives
typedef void(*PFNSRSUB675511)(char*, u32);
#define ff7InitLGPTableIdx  ((PFNSRSUB675511)0x675511)

#define BATTLE_LGP_PATH "\x43\x3A\x5C\x47\x61\x6D\x65\x73\x5C\x46\x46\x37\x5C\x64\x61\x74\x61\x5C\x62\x61\x74\x74\x6C\x65\x2F\x62\x61\x74\x74\x6C\x65\x2E\x6C\x67\x70\x0"


#pragma pack(push, 1)
typedef struct {
    char name[20];
    u32 fileOffset;
    u16 something;
    u8 something2;
} LGPTableEntry;
#pragma pack(pop)

typedef struct {
    char name[12];
    u32 fileCount;
    LGPTableEntry* lookupTable;
} LGPHeader;

#pragma pack(push, 1)
typedef struct {
    char name[20];
    u32 filelength;
    u8 rawDataStart;
} LGPFileHeader;
#pragma pack(pop)

//Size of actual LGP file and a ptr to the data in the pre-loaded lgp buffer
typedef struct {
    size_t size;
    void* buffer;
} LGPArchiveFile;


std::string lowerCaseStr(const std::string& stringToLower);
void setupLGPTable(char *filePath, u32 tableIdx);

void* readLGPArchive(const char* filepath);
LGPArchiveFile lgpArchiveRead(u8* lgpBuffer, const char* mangledName);
#endif // !LGP_LOADER_H
