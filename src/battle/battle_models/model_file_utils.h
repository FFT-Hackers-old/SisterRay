#ifndef MODEL_FILE_UTILS_H
#define MODEL_FILE_UTILS_H

#include <SisterRay/SisterRay.h>

typedef void(*PFNFF7MANGLER)(char*, char*);

typedef struct {
    u32 mode;
    u32 useLGP;
    u32 field_8;
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
    u32 field_8;
    u8 unk[0x38];
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

typedef void*(*PFNSRSUB6829FE)(LGPContext*, char*, char*);
#define ff7LoadModelFile   ((PFNSRSUB6829FE)0x6829FE)

typedef void(*PFNSRSUB5E1449)(char*, char*);
#define getBFileName       ((PFNSRSUB5E1449)0x5E1449)

#endif // !MODEL_FILE_UTILS_H
