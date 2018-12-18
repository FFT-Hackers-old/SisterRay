#ifndef SISTER_RAY_ROUTINES_H
#define SISTER_RAY_ROUTINES_H

#include <SisterRay/types.h>

#define FORMULA_TABLE_LOCATION          ((void*)0x8FF1F8)
#define PHYSICAL_DAMAGE_FORMULA         ((void*)0x5DE5DF)
#define MAGICAL_DAMAGE_FORMULA          ((void*)0x5DE9B8)
#define CURRENT_HP_GRAVITY              ((void*)0x5DEA6D)
#define MAX_HP_GRAVITY                  ((void*)0x5DEAF7)
#define CURE_FORMULA                    ((void*)0x5DEB81)
#define CONSTANT_20                     ((void*)0x5DEBE5)
#define CONSTANT_16                     ((void*)0x5DEC0A)
#define RECOVERY_FORMULA                ((void*)0x5DEC52)
#define THROW_FORMULA                   ((void*)0x5DEC8A)
#define COIN_FORMULA                    ((void*)0x5DECAA)
#define MAIN_COMMAND_FORMULA            ((void*)0x435D81)
#define APPLY_ELEMENTAL_MODIFIERS       ((void*)0x5DB593)
#define POISON_SETUP_ROUTINE            ((void*)0x5C9FC0)

typedef void(*pfnnullmasks)();
typedef void(*pfnenqueueaction)(u16, u16, u8, u8, u8);
typedef i32(*pfnsadnessmodifier)(i32);
typedef i32(*pfnsplitdamagequadramagic)(i32, u32);
typedef i32(*pfnbarriersmodifier)(i32);
typedef u32(*pfnrandomvariation)(i32);
typedef u32(*pfnmpturbo)(i32);
typedef void(*pfncommandmain)(u32);
typedef void(*pfnpoisonsetup)();

#define nullMasks                       ((pfnnullmasks)0x5DBCDD)
#define enqueueAction                   ((pfnenqueueaction)0x432687)
#define sadnessModifier                 ((pfnsadnessmodifier)0x5DE958)
#define spltDamageQuadraMagicModifier   ((pfnsplitdamagequadramagic)0x5DE8F4)
#define barriersModifier                ((pfnbarriersmodifier)0x5DE82C)
#define randomVariation                 ((pfnrandomvariation)0x5DE988)
#define mpTurbo                         ((pfnmpturbo)0x5DE958)
#define poisonSetUp                     ((pfnpoisonsetup)0x5C9FC0)

#endif
