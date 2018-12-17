#include <SisterRay/SisterRay.h>

static void* FORMULA_TABLE_LOCATION;
static void* PHYSICAL_DAMAGE_FORMULA;
static void* MAGICAL_DAMAGE_FORMULA;
static void* CURRENT_HP_GRAVITY;
static void* MAX_HP_GRAVITY;
static void* CURE_FORMULA;
static void* CONSTANT_20;
static void* CONSTANT_16;
static void* RECOVERY_FORMULA;
static void* THROW_FORMULA;
static void* COIN_FORMULA;
static void* MAIN_COMMAND_FORMULA;

//Damage Modifier Routines
static void* APPLY_ELEMENTAL_MODIFIERS;

typedef void(*pfnnullmasks)();
typedef void(*pfnenqueueaction)(u16, u16, u8, u8, u8);
typedef i32(*pfnsadnessmodifier)(i32);
typedef i32(*pfnsplitdamagequadramagic)(i32, u32);
typedef i32(*pfnbarriersmodifier)(i32);
typedef u32(*pfnrandomvariation)(i32);
typedef u32(*pfnmpturbo)(i32);
typedef void(*pfncommandmain)(u32);

static pfnnullmasks nullMasks;

/*function to enqueue a an action*/
static pfnenqueueaction enqueueAction;

static pfnsadnessmodifier sadnessModifier;

static pfnsplitdamagequadramagic spltDamageQuadraMagicModifier;

static pfnbarriersmodifier barriersModifier;

static pfnrandomvariation randomVariation;

static pfnmpturbo mpTurbo ;

static pfncommandmain commandMain;
