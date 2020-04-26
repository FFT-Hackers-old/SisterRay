#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include <SisterRay/SisterRay.h>
#include "model_file_utils.h"

typedef ModelSkeleton*(*PFNSRSUB5E11CC)(i32, i32, i32, AABone*, ModelAAHeader*, LGPLoadCtx *, FileContext*, char *);
#define createSkeleton  ((PFNSRSUB5E11CC)0x5E11CC)

#endif // !MODEL_SKELETON_H
