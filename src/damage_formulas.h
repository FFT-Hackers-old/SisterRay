#include <SisterRay/SisterRay.h>

void PhysicalFormulaRewrite();

typedef i32(*pfnsadnessmodifier)(i32);
pfnsadnessmodifier sadnessModifier = (pfnsadnessmodifier)0x5DE958;

typedef i32(*pfnsplitdamagequadramagic)(i32, u32);
pfnsplitdamagequadramagic spltDamageQuadraMagicModifier = (pfnsplitdamagequadramagic)0x5DE8F4;

typedef i32(*pfnbarriersmodifier)(i32);
pfnbarriersmodifier barriersModifier = (pfnbarriersmodifier)0x5DE82C;

typedef u32(*pfnrandomvariation)(i32);
pfnrandomvariation randomVariation = (pfnrandomvariation)0x5DE988;
