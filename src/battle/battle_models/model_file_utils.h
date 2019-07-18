#ifndef MODEL_FILE_UTILS_H
#define MODEL_FILE_UTILS_H

#include <SisterRay/SisterRay.h>
#include "../../files/lgp_loader.h"

#define BATTLE_LGP_PATH "data\\battle\\battle.lgp"

typedef void(*PFNFF7MANGLER)(const char*, char*);

typedef struct {
    u32 mode;
    u32 useLGP;
    u32 lgpTableIdx;
    PFNFF7MANGLER mangler;
} LGPContext;

typedef struct {
    u8 unk[64];
    LGPContext lgpContext;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
} FileContext;

typedef struct {
    u32 field_0;
    u32 field_4;
    u32 lgpTableIdx;
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 loadWeapon;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
} UnkModelLoadStruct;

typedef struct {
    u32 parent;
    float length;
    u32 hasParts;
} AABone;

typedef struct {
    u32 fileType;
    u32 loadBFileFlag;
    u32 initSkeletonFlag;
    u32 modelBoneCount;
    u32 field_10;
    u32 field_14;
    u32 textureCount;
    u32 modelAnimCount;
    u32 field_20;
    u32 weaponCount;
    u32 weaponAnimAcount;
    u32 field_2C;
    AABone* boneDataPtr;
} ModelAAHeader;

//This header is copied into the models, 
typedef struct {
    u16 characterID;
    u8 lepad[34];
    u32 unkFileOffsets[8]; //There are 8 pointers to a block after the animation scripts (end of file for model animations)
    u8 unk[32];
} ModelABHeader;

typedef struct{
    ModelABHeader header;
    u32 offsets[74];
    void* scriptData;
} ABFile;

typedef struct {
    u32 daFileModelAnimCount;
    u32 daFileWeaponAnimCount;
} DaFileCounts;

typedef struct {
    u32 bonesCount;
    u32 framesCount;
    u32 compressedSize;
} DaAnimHeader;

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

void* srLoadLGPFile(LGPContext* context, int* bytesReadBuf, char* filename);
LGPArchiveFile srOpenDAFile(LGPContext* context, const char* baseFileName, void* battleLGPBuffer);
LGPArchiveFile srOpenABFile(LGPContext* context, const char* baseFileName, void* battleLGpBuffer);
void* srGameLoadABFile(LGPContext* context, char* baseFileName);
void srGameInitAnimations(u32 lgpIndex, u32 unkint, void** srAnimsTable, LGPContext* context, char* filename);

typedef void(*PFNSRSUB5E1449)(const char*, char*);
#define createABFilename       ((PFNSRSUB5E1449)0x5E1449)

typedef void(*PFNSRSUB5E826A)(u32, const char*, char*);
#define createDAFilename   ((PFNSRSUB5E826A)0x5E826A)

#endif // !MODEL_FILE_UTILS_H
