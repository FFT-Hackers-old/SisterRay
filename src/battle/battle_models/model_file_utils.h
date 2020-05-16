#ifndef MODEL_FILE_UTILS_H
#define MODEL_FILE_UTILS_H

#include <SisterRay/SisterRay.h>
#include "../../files/lgp_loader.h"
#include <unordered_map>
#include <vector>
#include <string>

#define BATTLE_LGP_PATH "data\\battle\\battle.lgp"
#define MAGIC_LGP_PATH "data\\battle\\magic.lgp"

typedef void(*PFNSRSUB5DFEC8)(LGPContext*, char*);
#define noLGPgetDirectory    ((PFNSRSUB5DFEC8)0x5DFEC8)

typedef void(*PFNSRSUB5DFF15)(LGPContext*, char*);
#define noLGPClearContext    ((PFNSRSUB5DFF15)0x5DFF15)

typedef void(*PFNSRSUB5DFE08)(char*);
#define getFF7Directory      ((PFNSRSUB5DFE08)0x5DFE08)

typedef void(*PFNSRSUB5DFE51)(char*);
#define clearDirectory      ((PFNSRSUB5DFE51)0x5DFE51)

typedef void*(*PFNSRSUB6829FE)(LGPContext*, int*, char*);
#define ff7LoadModelFile   ((PFNSRSUB6829FE)0x6829FE)

typedef void*(*PFNSRSUB5E82DE)(u32, u32, void**, LGPContext*, char*);
#define ff7InitAnimations   ((PFNSRSUB5E82DE)0x5E82DE)

typedef ModelAAHeader*(*SRPFNSUB5E0FC0)(u32, LGPContext*, char*);
#define loadModelAAHeader    ((SRPFNSUB5E0FC0)0x5E0FC0)

typedef void(*SRPFNSUB429A12)(u32, u32, LGPLoadCtx*, FileContext*);
#define initLoadCtx          ((SRPFNSUB429A12)0x429A12)

typedef void(*SRPFNSUB5DFDC0)(LGPLoadCtx*);
#define clearLoadCtx       ((SRPFNSUB5DFDC0)0x5DFDC0)

typedef void(*SRPFNSUB5E0EED)(u32, ModelAAHeader*);
#define destroyModelAAHeader ((SRPFNSUB5E0EED)0x5E0EED)

typedef void(*SRPFNSUB67455E)(FileContext*);
#define initializeFileContext  ((SRPFNSUB67455E)0x67455E)

typedef void(*PFNSRSUB674659)(u32, FileContext*);
#define setSomeFileContext  ((PFNSRSUB674659)0x674659)

typedef void(*PFNSR_SUB5E17FE)(FileContext*);
#define setFileContext  ((PFNSR_SUB5E17FE)0x5E17FE)

void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename);
LGPArchiveFile srOpenDAFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer, bool doMangle=true);
LGPArchiveFile srOpenABFile(LGPContext* context, const char* baseFileName, void* battleLGpBuffer);
void* srGameLoadABFile(LGPContext* context, char* baseFileName);
void srGameInitAnimations(u32 lgpIndex, u32 unkint, void** srAnimsTable, LGPContext* context, char* filename);
std::vector<std::unordered_map<std::string, SrAnimation>> loadModelAnimationFromDAFile(const char* modelName, void* daFileBuffer, bool hasWeapon);

typedef void(*PFNSRSUB5E1449)(const char*, char*);
#define createABFilename       ((PFNSRSUB5E1449)0x5E1449)

typedef void(*PFNSRSUB5E826A)(u32, const char*, char*);
#define createDAFilename   ((PFNSRSUB5E826A)0x5E826A)

#endif // !MODEL_FILE_UTILS_H
