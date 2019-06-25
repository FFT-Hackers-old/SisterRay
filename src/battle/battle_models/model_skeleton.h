#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include <SisterRay/SisterRay.h>

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    float* matrix0;
    float* matrix1;
    float* matrix2;
} MatrixStruct;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct{
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    MatrixStruct* matrixStructPtr;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    u32 field_8C;
    u32 field_90;
    u32 field_94;
    u32 field_98;
    u32 field_9C;
    u32 field_A0;
    u32 field_A4;
    u32 field_A8;
    u32 field_AC;
} TexMatrices;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    TexMatrices* texMatrices;
    u32 field_8;
} FF7Texture;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    void* pDataPtr;
    FF7Texture* texDataPtr;
} PolygonData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 boneRoot;
    float boneLength;
    u32 partAttached;
} BoneConnection;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 field_4;
    BoneConnection boneConnection;
    u32 field_14;
    PolygonData* polygonData;
} ModelBone;
#pragma pack(pop)

/*Structure representing weapons AND battle model skeletons*/
#pragma pack(push, 1)
typedef struct{
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 totalBones;
    ModelBone* bonesArray;
    u32 unkStuff;
    u32 field_1C;
    void* ffContextPtr;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    u32 field_8C;
    u32 field_90;
    u32 field_94;
    u32 field_98;
    u32 field_9C;
    u32 field_A0;
    u32 field_A4;
} ModelSkeleton;
#pragma pack(pop)

typedef ModelSkeleton*(*PFNSRSUB5E11CC)(i32, i32, i32, AABone*, ModelAAHeader*, UnkModelLoadStruct *, FileContext*, char *);
#define createSkeleton  ((PFNSRSUB5E11CC)0x5E11CC)

#endif // !MODEL_SKELETON_H
