#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "data_structures.h"
#include "animations.h"
#include "graphics.h"
#include "game_module.h"

#pragma pack(push, 1)
typedef struct {
    MatrixSet matrixSet;
    PolygonSet* polygonSet;
    void* pdData;
} BoneRSD;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    void* pDataPtr;
    BoneRSD* boneRSDPtr;
} BonePolygons;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 parentIdx;
    float boneLength;
    u32 partAttached;
} BoneConnection;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    u8* boneName;
    u8* boneParent;
    BoneConnection boneConnection;
    char** rsdNames;
    BonePolygons* polygons;
} ModelBone;
#pragma pack(pop)

/*Structure representing weapons AND battle model skeletons*/
#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 field_4;
    u32 debugActive;
    u32 skeletonFlags;
    u32 totalBones;
    ModelBone* bonesArray;
    u32 field_18;
    GameList* boneList;
    GameContext* gameContext;
    Matrix field_24;
    Matrix field_64;
    u32* field_A4;
} ModelSkeleton;
#pragma pack(pop)

typedef struct {
    void* animScriptStruct;            //0x00
    ModelSkeleton* skeletonData;       //0x04
    u32 totalAnimations;               //0x08
    BattleAnimation** animationsTable; //0x0C
    u32 limitCount;                    //0x10
    void* limitsTable;                 //0x14
    u32 weaponCount;                   //0x18
    ModelSkeleton** weaponModels;      //0x1C
    float matrixMeta[11];              //0x20
    float modelMatrix[16];             //initialized to identity element of GL_4(R) on model create
    float unk1;
    float unk2;
} BattleModel;

#pragma pack(push, 1)
typedef struct {
    u16 xCoordinate;
    u16 yCoordinate;
    u16 zCoordinate;
} ModelPosition;
#pragma pack(pop)

/*Should  have size 0x1AEC*/
#pragma pack(push, 1)
typedef struct {
    u16 characterID; //BE1170, 0
    u16 animScriptIndex; //BE1172, 2
    u8 padding1[4]; //BE1174, 4
    u16 AnimationData; //BE1178, 8
    u16 animScriptPtr; //BE117A, 0xA
    u16 field_C;
    u16 runningAnimIdx; //0xE
    u16 field_10;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
    u16 field_20;
    u8 animationEffect; //BE119A, 0x22
    u8 commandID; //BE119B, 0x23
    u8 field_24;
    u8 field_25;
    u8 field_26;
    u8 field_27;
    u8 field_28;
    u8 unkActorFlags;
    u8 field_2A;
    u8 bData0x12[16]; //0x2B
    u8 isScriptExecuting; //0x3B
    u8 currentScriptPosition; //0x3C
    u8 waitFrames;            //0x3D
    u8 modelEffectFlags;      //0x3E
    u8 padding3[0x11F];       //0x3F
    u16 field_15E;
    u16 field_160;
    u16 field_162;
    u16 field_164;
    ModelPosition restingPosition; //0x166
    u8 padding5[0xA30];
    u32 playedAnimFrames;
    u32 currentPlayingFrame;
    u32 tableRelativeModelAnimIdx;
    BattleModel* modelDataPtr;
    u8 padding4[0xF17];
    u32 setForLimitBreaks;
    u8 padd5[37];
} BattleModelState;
#pragma pack(pop)

#define BATTLE_MODEL_STATE_BIG_ARRAY       ((BattleModelState*)0xBE1178)

#pragma pack(push, 1)
typedef struct {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    u16 bData68[4];
    u16 field_C;
    u16 bData76[6];
    u16 bData88[6];
    u16 field_26;
    u16 field_28;
    u16 field_2A;
    u8 field_2C;
    u8 field_2D;
    u16 field_2E;
    u16 field_30;
    u8 modelDataIndex; //0x032
    u8 field_32;
    u8 field_33;
    u8 field_34;
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
    u8 field_3A;
    u8 field_3B;
    u8 field_3C;
    u8 field_3D;
    u8 field_3E;
    u8 field_3F;
    u8 field_40;
    u8 field_41;
    u8 field_42;
    u8 field_43;
    u8 field_44;
    u8 field_45;
    u8 field_46;
    u8 field_47;
    u8 field_48;
    u8 field_49;
    u8 field_4A;
    u8 field_4B;
    u8 field_4C;
    u8 field_4D;
    u8 field_4E;
    u8 field_4F;
    u8 field_50;
    u8 field_51;
    u8 field_52;
    u8 field_53;
    u8 field_54;
    u8 field_55;
    u8 field_56;
    u8 field_57;
    u8 field_58;
    u8 field_59;
    u8 field_5A;
    u8 field_5B;
    u8 field_5C;
    u8 field_5D;
    u8 field_5E;
    u8 field_5F;
    u8 field_60;
    u8 field_61;
    u8 field_62;
    u8 field_63;
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    u8 field_68;
    u8 field_69;
    u8 field_6A;
    u8 field_6B;
    u8 field_6C;
    u8 field_6D;
    u8 field_6E;
    u8 field_6F;
    u8 field_70;
    u8 field_71;
    u8 field_72;
    u8 field_73;
} BattleModelStateSmall;
#pragma pack(pop)

#define BATTLE_MODEL_STATE74_ARRAY ((BattleModelStateSmall*)0xBF23C0)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 field_4;
    GameRotationMatrix rotationMatrix;
    u16 field_26;
    u32 field_28;
    u16 field_2C;
    u16 field_2E;
    u16 field_30;
    u16 field_32;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
} ModelRotationData;
#pragma pack(pop)

#define BATTLE_MODEL_ROTATION_DATA ((ModelRotationData*)0xBE0E28)

#endif // !MODEL_H
