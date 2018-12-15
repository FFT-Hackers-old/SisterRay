#include <SisterRay/SisterRay.h>

static void* FORMULA_TABLE_LOCATION = (void*)0x8FF1F8;
static void* PHYSICAL_DAMAGE_FORMULA = (void*)0x5DE5DF;
static void* MAGICAL_DAMAGE_FORMULA = (void*)0x5DE9B8;
static void* CURRENT_HP_GRAVITY = (void*)0x5DEA6D;
static void* MAX_HP_GRAVITY = (void*)0x5DEAF7;
static void* CURE_FORMULA = (void*)0x5DEB81;
static void* CONSTANT_20 = (void*)0x5DEBE5;
static void* CONSTANT_16 = (void*)0x5DEC0A;
static void* RECOVERY_FORMULA = (void*)0x5DEC52;
static void* THROW_FORMULA = (void*)0x5DEC8A;
static void* COIN_FORMULA = (void*)0x5DECAA;

//Damage Modifier Routines
static void* APPLY_ELEMENTAL_MODIFIERS = (void*)0x5DB593;

typedef void(*pfnnullmasks)();
static pfnnullmasks nullMasks = (pfnnullmasks)0x5DBCDD;

/*function to enqueue a an action*/
typedef void(*pfnenqueueaction)(u16, u16, u8, u8, u8);
static pfnenqueueaction enqueueAction = (pfnenqueueaction)0x432687;

typedef i32(*pfnsadnessmodifier)(i32);
static pfnsadnessmodifier sadnessModifier = (pfnsadnessmodifier)0x5DE958;

typedef i32(*pfnsplitdamagequadramagic)(i32, u32);
static pfnsplitdamagequadramagic spltDamageQuadraMagicModifier = (pfnsplitdamagequadramagic)0x5DE8F4;

typedef i32(*pfnbarriersmodifier)(i32);
static pfnbarriersmodifier barriersModifier = (pfnbarriersmodifier)0x5DE82C;

typedef u32(*pfnrandomvariation)(i32);
static pfnrandomvariation randomVariation = (pfnrandomvariation)0x5DE988;

typedef u32(*pfnmpturbo)(i32);
static pfnmpturbo mpTurbo = (pfnmpturbo)0x5DE958;
