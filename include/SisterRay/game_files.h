#ifndef GAME_FILES_H
#define GAME_FILES_H

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
    u32 totalBones;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 loadWeapon;
    u32 someMPCopy;
    u32 statusImmuneMask;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
} LGPLoadCtx;

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
    u32 totalBones;
    u32 field_14;
    u32 textureCount;
    u32 modelAnimCount;
    u32 field_20;
    u32 weaponCount;
    u32 weaponAnimAcount;
    u32 someHPCopy;
    AABone* boneDataPtr;
} ModelAAHeader;

//This header is copied into the models, 
typedef struct {
    u16 characterID;
    u8 lepad[34];
    u32 unkFileOffsets[8]; //There are 8 pointers to a block after the animation scripts (end of file for model animations)
    u8 unk[32];
} ModelABHeader;

typedef struct {
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
#endif // !GAME_FILES_H
