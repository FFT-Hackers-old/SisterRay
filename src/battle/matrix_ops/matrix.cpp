#include "matrix.h"


typedef void(*PFNSR_ACTORMATCOPY)(u8, u16, GameRotationMatrix*);
static const auto gameCopyIndexedActorMatrix = (PFNSR_ACTORMATCOPY)0x4293B8;
void srCopyWeaponMatrix(u8 actorIdx, u16 matrixIdx, GameRotationMatrix* ret) {
    gameCopyIndexedActorMatrix(actorIdx, matrixIdx, ret);
}


typedef void(*PFNSR_DELTATRANSPOSE)(GameRotationMatrix*, GameRotationMatrix*, GameRotationMatrix*);
static const auto gameApplyDeltaTranspose = (PFNSR_DELTATRANSPOSE)0x5D2A30;
void srApplyDeltaTranspose(GameRotationMatrix* a, GameRotationMatrix* b, GameRotationMatrix* ret) {
    gameApplyDeltaTranspose(a, b, ret);
}

typedef void(*PFNSR_TRANSFORMMATRIX)(GameRotationMatrix*);
static const auto gameCopyR3Rotation = (PFNSR_TRANSFORMMATRIX)0x663673;
//Writes to the global matrix computation buffer
void srCopyRotMatToGlobal(GameRotationMatrix* src) {
    gameCopyR3Rotation(src);
}

static const auto gameCopyTranslation = (PFNSR_TRANSFORMMATRIX)0x663707;
void srCopyTranslationToGlobal(GameRotationMatrix* src) {
    gameCopyTranslation(src);
}

typedef void(*PFNSR_COMPUTEABSPOS)(R3PointWord*, R3PointDWord*, u32*);
static const auto gameComputeAbsPosition = (PFNSR_COMPUTEABSPOS)0x662ECC;
void srComputeAbsolutePosition(R3PointWord* base, R3PointDWord* relative, u32* w) {
    gameComputeAbsPosition(base, relative, w);
}


typedef void (*SRPFN_WORDMATMULTIPLY)(GameRotationMatrix*, GameRotationMatrix*, GameRotationMatrix*);
static const auto gameMatrixProduct = (SRPFN_WORDMATMULTIPLY)0x661E85;
void srMatrixProduct(GameRotationMatrix* a, GameRotationMatrix* b, GameRotationMatrix* ret) {
    gameMatrixProduct(a, b, ret);
}
