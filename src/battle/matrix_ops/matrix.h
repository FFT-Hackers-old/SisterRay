#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <SisterRay/SisterRay.h>


void srCopyWeaponMatrix(u8 actorIdx, u16 matrixIdx, GameRotationMatrix* ret);
void srApplyDeltaTranspose(GameRotationMatrix* a, GameRotationMatrix* b, GameRotationMatrix* ret);
void srCopyRotMatToGlobal(GameRotationMatrix* src);
void srCopyTranslationToGlobal(GameRotationMatrix* src);
void srComputeAbsolutePosition(R3PointWord* base, R3PointDWord* relative, u32* w);
void srMatrixProduct(GameRotationMatrix*, GameRotationMatrix*, GameRotationMatrix*);
#endif // !MATRIX_OPS_H
