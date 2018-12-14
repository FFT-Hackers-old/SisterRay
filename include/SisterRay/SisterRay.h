#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stdint.h>

//Define relevant constants for accessing specific parts of the exe*//

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;

void* FORMULA_TABLE_LOCATION = (void*)0x8FF1F8;

void* PHYSICAL_DAMAGE_FORMULA = (void*)0x5DE5DF;
void* MAGICAL_DAMAGE_FORMULA = (void*)0x5DE9B8;
void* CURRENT_HP_GRAVITY = (void*)0x5DEA6D;
void* MAX_HP_GRAVITY = (void*)0x5DEAF7;
void* CURE_FORMULA = (void*)0x5DEB81;
void* CONSTANT_20 = (void*)0x5DEBE5;
void* CONSTANT_16 = (void*)0x5DEC0A;
void* RECOVERY_FORMULA = (void*)0x5DEC52;
void* THROW_FORMULA = (void*)0x5DEC8A;
void* COIN_FORMULA = (void*)0x5DECAA;

//Damage Modifier Routines
void* APPLY_ELEMENTAL_MODIFIERS = (void*)0x5DB593;

//This struct has size 260h and is referenced by the main context ptr
#pragma pack(push, 1)
typedef struct {
    u32 attackerID; //0x00
    u32 attackerLevel; //0x04
    u32 attackerEnemyIndex; //0x08
    u32 actionId; //0x0C
    u32 attackIndex; //0x10
    u32 attackerIDMagic; //0x14
    u32 targetMask; //0x18
    u32 activeAllies; //0x1C
    u32 animationScriptID; //0x20
    u32 AttackEffectID; //0x24
    u32 actionIDCopy; //0x28
    u32 attackIDCopy; //0x2C
    u32 attackerMask; //0x30
    u32 partofAttackerMask; //034
    u32 MPCost; //0x38
    u32 hitChance; //0x3C
    u32 damageFormulaID; //0x44
    u32 attackElementsMask; //0x44
    u32 abilityPower; //0x48
    i32 attackerAtk; //0x4C
    u32 targetStateMask; //0x50
    u32 normalAtkSound; //0x54
    u32 critAtkSound; //0x58
    u32 missAtkSound; //0x5C
    u32 cameraDataSingle; //0x60
    u32 cameraDataMultiple; //0x64
    u32 impactEffectID; //0x68
    u32 specialAbilityFlags; //0x6C
    u8 padding1[8]; //0x70, 0x74
    u32 damageCalcMask; //0x78
    u32 addStatusMask; //0x80
    u32 rmStatusMask; //0x84
    u32 changeStatusMask; //0x88
    u32 inflictStatusChange; //0x8C
    u32 miscActionflags; //0x90
    u32 targetMaskCopy; //0x94
    u32 attackIndexCopy; //0x98
    u8 padding2[4];
    u32 formulaType; //0xA0
    u32 formulaID; //0xA4
    u32 SenseFlag; //0xA8
    u32 unkDWord1; //0xAC
    u32 attackHitCount; //0xB0
    u32 doubleCutRelated; //0xB4
    u32 targetCount; //0xB8
    u32 additionalEffects; //0xBC
    u32 additionalEffectsModifer; //0xC0
    u32 abilityPowerCopy; //0xC4
    u32 attackerStatusMask; //0xC8
    u32 miscAttackData; //0xCC
    u32 unkDWord2; //0xD0
    u32 unkDWord3; //0xD4
    u32 throwFormulaPower; //0xD8
    u32 displayString; //0xDC
    u32 incOnDamageDealt; //0xE0
    u32 unkDWord4; //0xE4
    u32 MPTurboBoost; //0xE8
    u32 targetMaskCopy2; //0xEC
    u32 addedCutMPHPAbsorbByte; //0xF0
    u32 currentUnitIDTempt; //0xF4
    u32 unkDWord5; //0xF8
    u32 hitCountCopy; //0xFC
    /*256 unused bytes*/
    u8 padding3[100]; 
    u32 targetObjectBlock1Ptr; //0x200
    u32 targetObjectBlock2Ptr; //0x204
    u32 targetID; //0x208
    u32 targetEnemyIndex; //0x20C
    u32 targetDefense; //0x210
    i32 currentDamage; //0x214
    u32 abilityFlags1; //0x218
    u32 unkDWord6; //0x21C
    u32 abilityFlags2; //0x220
    u32 unkActionID; //0x224
    u32 targetStatusMask; //0x228
    u32 targetStatusImmuneMask; //0x22C
    u32 elementalDefenseMask; //0x230
    u32 targetStateFlags; //0x234
    u32 addStatusMaskCopy; //0x238
    u32 rmStatusMaskCopy; //0x23C
    u32 changeStatusMaskCopy; //0x240
    u32 combinedStatusMask; //0x244
    u32 attackImpactSound; //0x248
    u32 attackImpactEffectID; //0x24C
    u32 killRecoverDamage; //0x250
    u32 targetLevel; //0x254
    u32 targetCurrentHP; //0x258
    u32 targeCurrentMP; //0x25C
    u32 hitChanceCopy; //0x260
} DamageCalcStruct;
#pragma pack(pop)

//An array of size ACTOR_ARRAY_SIZE w/ elements of size 0x68 exists at ARRAY_ACTOR_START
#pragma pack(push, 1)
typedef struct {
    u32     statusMasks;
    u32     stateFlags;
    u8      index;
    u8      level;
    u8      unknown0;
    u8      elementDamageMask;
    u8      characterID;
    u8      physAtk;
    u8      magAtk;
    u8      pEvade;
    u8      idleAnimID;
    u8      damageAnimID;
    u8      backDamageMult;
    u8      sizeScale;
    u8      dexterity;
    u8      luck;
    u8      idleAnimHolder;
    u8      lastCovered;
    u16     lastTargets;
    u16     prevAttackerMask;
    u16     prevPhysAttackerMask;
    u16     prevMagAttackerMask;
    u16     Defense;
    u16     MDefense;
    u16     formationID;
    u16     absorbedElementsMask;
    u16     currentMP;
    u16     maxMP;
    i32     currentHP;
    i32     maxHP;
    u32     unknown1;
    u32     unknown2;
    u32     unknown3;
    u32     unknown4;
    u32     initalStatusMasks;
    u32     unknown5;
    u8      unknown6;
    u8      mEvade;
    u8      actorRow;
    u8      cameraData;
    u16     gilStolen;
    u16     itemStolen;
    u16     unknown7;
    u16     apValue;
    u32     gilValue;
    u32     expValue;
    u8      unused8;
    u8      unused9;
    u8      unused10;
    u8      unused11;
    u8      unused12;
    u8      unused13;
    u8      unused14;
    u8      unused15;

} ActorBattleVars;
#pragma pack(pop)

DamageCalcStruct* const DamageContextPtr = (DamageCalcStruct*)0x99CE0C;

const ActorBattleVars* AIActorVariables = (ActorBattleVars*)0x9AB0DC;
int ACTOR_ARRAY_SIZE = 10;

#endif
