#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include <SisterRay/SisterRay.h>

typedef void(*PFNSRSUB5E11CC)(i32, i32, i32, AABone*, ModelAAHeader*, UnkModelLoadStruct *, FileContext, char *);
#define createSkeleton  ((PFNSRSUB5E11CC)0x5E11CC)

#endif // !MODEL_SKELETON_H
