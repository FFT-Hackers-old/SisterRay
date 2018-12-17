#ifndef SR_ROUTINES_H
#define SR_ROUTINES_H

#include <SisterRay/routines.h>

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
static void* MAIN_COMMAND_FORMULA = (void*)0x435D81;

//Damage Modifier Routines
static void* APPLY_ELEMENTAL_MODIFIERS = (void*)0x5DB593;

#endif
