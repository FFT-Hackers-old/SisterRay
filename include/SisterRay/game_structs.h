#ifndef SISTER_RAY_GAME_STRUCTS_H
#define SISTER_RAY_GAME_STRUCTS_H

#include <SisterRay/types.h>


#pragma pack(push, 1)
typedef struct {
    u8 attackerActorID;
    u8 actionCommandIndex;
    u16 actionAttackIndex;
    u16 actionTargetMask;
} QueueAction;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u8 entryPriority;
    u8 entryOffset;
    QueueAction queueAction;
} BattleQueueEntry;
#pragma pack(pop)

//This struct has size 260h and is referenced by the main context ptr
#pragma pack(push, 1)
typedef struct {
    u32 attackerID; //0x00
    u32 attackerLevel; //0x04
    u32 enemySceneIndex; //0x08 This is the relative index in the scene, i.e 0 1 or 2 of a given actor
    u32 commandIndex; //0x0C
    u32 relAttackIndex; //0x10
    u32 enabledMagicsIndex; //0x14
    u32 targetMask; //0x18
    u32 activeAllies; //0x1C
    u32 animationScriptID; //0x20
    u32 animationEffectID; //0x24
    u32 commandIndexCopy; //0x28
    u32 absAttackIndex; //0x2C
    u32 attackerMask; //0x30
    u32 partofAttackerMask; //034
    u32 MPCost; //0x38
    u32 abilityHitRate; //0x3C
    u32 damageFormulaID; //0x40
    u32 attackElementsMask; //0x44
    u32 abilityPower; //0x48
    i32 attackerAtk; //0x4C
    u32 abilityTargetingFlags; //0x50
    u32 impactSound; //0x54
    u32 critAtkSound; //0x58
    u32 missAtkSound; //0x5C
    u32 cameraDataSingle; //0x60
    u32 cameraDataMultiple; //0x64
    u32 impactEffectID; //0x68
    u32 specialAbilityFlags; //0x6C
    u8 padding1[8]; //0x70, 0x74
    u32 wasDamagedMask; //0x78
    u32 wasKilledMask; // 0x7C
    u32 addStatusMask; //0x80
    u32 rmStatusMask; //0x84
    u32 toggleStatusMask; //0x88
    u32 inflictStatusChance; //0x8C
    u32 miscActionFlags; //0x90
    u32 finalTargetMask; //0x94
    u32 sceneAbilityIndex; //0x98
    u8 padding2[4];
    u32 formulaType; //0xA0
    u32 formulaID; //0xA4
    u32 actionCounterable; //0xA8
    u32 quadCount; //0xAC
    u32 attackHitCount; //0xB0
    u32 actionCount; //0xB4
    u32 targetCount; //0xB8
    u32 additionalEffects; //0xBC
    u32 additionalEffectsModifer; //0xC0
    u32 abilityPowerCopy; //0xC4
    u32 attackerStatusMask; //0xC8
    u32 targetReactionAnimation; //0xCC
    u8 followUpActions[8]; //0xD0
    u32 throwFormulaPower; //0xD8
    u32 displayString; //0xDC
    u32 incOnDamageDealt; //0xE0
    u32 unkDword8; //0xE4
    u32 MPTurboBoost; //0xE8
    u32 usedTargetMask; //0xEC
    u32 supportMatFlags; //0xF0
    u32 currentUnitIDTempt; //0xF4
    u32 quadEnabled; //0xF8
    u32 hitCountCopy; //0xFC
    /*256 unused bytes*/
    u8 padding3[0x100];
    u32 targetObjectBlock1Ptr; //0x200
    u32 targetObjectBlock2Ptr; //0x204
    u32 targetID; //0x208
    u32 targetEnemyIndex; //0x20C
    u32 targetDefense; //0x210
    u32 currentDamage; //0x214
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
    u32 finalHitRate; //0x260
} DamageCalcStruct;
#pragma pack(pop)

#define gDamageContextPtr   ((DamageCalcStruct*)0x0099E308)

//An array of size ACTOR_ARRAY_SIZE w/ elements of size 0x68 exists at ARRAY_ACTOR_START
#pragma pack(push, 1)
typedef struct {
    u32     statusMask;        //0x00
    u32     stateFlags;        //0x04
    u8      index;             //0x08
    u8      level;             //0x09
    u8      unknown0;          //0x0A
    u8      elementDamageMask; //0x0B
    u8      characterID;       //0x0C
    u8      physAtk;           //0x0D
    u8      magAtk;            //0x0E
    u8      pEvade;            //0x0F
    u8      idleAnimID;        //0x10
    u8      damageAnimID;      //0x11
    u8      backDamageMult;    //0x12
    u8      sizeScale;         //0x13
    u8      dexterity;         //0x14
    u8      luck;              //0x15
    u8      idleAnimHolder;    //0x16
    u8      lastCovered;       //0x17
    u16     lastTargets;       //0x18
    u16     prevAttackerMask;  //0x1A
    u16     prevPhysAttackerMask; //0x1C
    u16     prevMagAttackerMask;  //0x1E
    u16     Defense;           //0x20
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
    u16     unused10; //This is being used for the tactical elements mod, unused in original game
    u8      unused12;
    u8      unused13;
    u8      unused14;
    u8      unused15;
} ActorBattleVars;
#pragma pack(pop)

#define gAiActorVariables   ((ActorBattleVars*)0x9AB0DC)

#pragma pack(push, 1)
typedef struct {
    u8 lastCommandIdx; //00
    u8 lastActionIdx;  //08
    u8 bankAccessValue; //10
    u8 dummyByte;   //18
    u8 battleType;  //20
    u8 unk1; //28
    u8 unk2; //30
    u8 limitLevel; //38
    u8 unk3; //40
    u8 unk4; //48
    u8 pad;
    u16 activeActorMask; //50
    u16 scriptOwnerMask;  //60
    u16 actionTargetMask;  //70
    u16 actorAlliesMask;   //80
    u16 activeAlliesMask;  //90
    u16 actorEnemiesMask;  //A0
    u16 activeEnemiesMask;  //B0
    u16 actorPartyMask;     //C0
    u16 unkMask;            //D0
    u16 allActorsMask;      //E0
    u16 unkMask2;            //F0
    u16 unkMask3;            //0x100
    u16 endBattleFlags;      //2110
    u16 lastActionElements;  //2120
    u16 unkDword3;           //130
    u16 battleFormationIdx;  //140
    u16 lastAbsActionIdx;   //150
    u16 unkBattleFlags;      //160
    u16 specialAttackFlags;  //170
    u16 unkLimitDivisor;     //180
    u16 unkDword;            //190
    u16 stringArgs;
    u16 somethingEmerald;    //1A0
    u32 partyGil;            //1C0
    ActorBattleVars actorAIStates[10];
} AIBattleContext;
#pragma pack(pop)


#define AI_BATTLE_CONTEXT   ((AIBattleContext*)0x9AB0A0)

#pragma pack(push)
typedef struct {
    u32 flags;
    u32 unk1;
    u32 unk2;
    u32 unk3;
} PlayerFlagCopies;
#pragma pack(pop)

#define PLAYER_FLAG_COPIES  ((PlayerFlagCopies*)0x9A8800)

#pragma pack(push, 1)
typedef struct {
    u16 vTimerIncrement;       //0x00
    u16 yurnTimerIncrement;    //0x02
    u16 charATBValue;          //0x04
    u16 field_6;               //0x06
    u16 field_8;
    u16 CTimerIncrement;       //0xA
    u8 field_C;
    u8 field_D;
    u8 activeCommandsMask;     //0x0D
    u8 field_F;                //0x0F
    u8 field_10;               //0x10
    u8 field_11;               //0x11
    u8 field_12;
    u8 field_13;
    u8 field_14;
    u8 field_15;               //0x15
    u8 field_16;               //0x16
    u8 poisonTick;             //0x17
    u8 field_18;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
    u8 field_24;
    u8 field_25;
    u8 field_26;
    u8 field_27;
    u8 reflectCount;
    u8 unkActorFlags;           //0x29
    u8 field_2A;                //0x2A
    u8 field_2B;                //0x2B
    u32 field_2C;               //0x2C
    u32 field_30;               //0x30
    u32 statusImmuneMask;       //0x34
    u32 field_38;               //0x38
    u16 currentHP;              //0x3C
    u16 currentMP;              //0x3E
    u8 field_40;                //0x40
    u8 field_41;
    u8 field_42;
    u8 field_43;
} ActorTimerData;
#pragma pack(pop)

#define G_ACTOR_TIMER_ARRAY ((ActorTimerData*)(0x9A8B10))

#pragma pack(push, 1)
typedef struct {
    CharacterRecord* characterRecord;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u16 limitBreakBar;
    u8 field_A;
    u8 field_B;
    u8 field_C;
    u16 field_E;
    u16 maxMP;
    u16 maxHP;
    u16 field_14; //Seems to be HP/MP switch HP
    u16 field_16; //Seems to be HP/MP switch MP
    u16 previousSupportMasks[2];
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
    u32 field_20;
    u32 learnedEnemySkills;
    u32 knownEnemySkills;
    u8 field_2C;
    u8 field_2D;
    u8 field_2F;
    u8 field_30;
    u8 field_31;
    u8 field_32;
    u8 field_33;
}BattleParty34;
#pragma pack(pop)

#define G_BATTLE_PARTY34_ARRAY  ((BattleParty34*)(0x09A8DB8))

#pragma pack(push, 1)
typedef struct {
    u8 characterID; //0x00
    u8 field_1;    //0x01
    u8 field_2;    //0x02
    u8 field_3;    //0x03
    u8 deathType;    //0x04
    u8 field_5;       //0x05
    u16 field_6;    //0x06
    u32 field_8;   //0x08
    u32 field_C;   //0x0C
} BattleParty10;
#pragma pack(pop)

#define G_BATTLE_PARTY10_ARRAY ((BattleParty10*)0x9A87F4)

#pragma pack(push, 1)
typedef struct {
    u8 targetFlags; //0x00
    u8 attackEffectID;    //0x01
    u8 damageCalculation;    //0x02
    u8 weaponHitRat;    //0x03
    u8 impactEffectID;    //0x04
    u8 criticalRat;       //0x05
    u16 padding;
    u16 normalSoundID;
    u16 critSoundID;
    u16 missSoundID;
    u16 attackElementMask;
    u16 cameraMask;
    u16 specialAttackFlags;
    u32 attackStatusMask;
} BattleWeaponCtx;
#pragma pack(pop)

#define G_BATTLE_WEAPON_ARRAY ((BattleWeaponCtx*)0x9A8E54)

#pragma pack(push, 1)
typedef struct {
    u8 characterID;
    u8 level;
    u8 strength;
    u8 vitality;
    u8 magic;
    u8 spirit;
    u8 dexterity;
    u8 luck;
    u8 bonus_strength;
    u8 bonus_vitality;
    u8 bonus_magic;
    u8 bonus_spirit;
    u8 bonus_dexterity;
    u8 bonus_luck;
    u8 active_limit_level;
    u8 limit_bar_progress;
    char character_name[12];
    u8 equipped_weapon;
    u8 equipped_armor;
    u8 equipped_accessory;
    u8 out_of_battle_status;
    u8 character_row;
    u8 level_progress;
    u16 learned_limits;
    u16 character_kills;
    u16 limit_1A_uses;
    u16 limit_2A_uses;
    u16 limit_3A_uses;
    u16 current_HP;
    u16 base_HP;
    u16 current_MP;
    u16 base_MP;
    u8 unknown[4];
    u16 max_HP;
    u16 max_MP;
    u32 current_EXP;
    u32 equippedWeaponMateria[8]; //This is primarily written to/from here, so this should be relocated if we are to expand 
    u32 equippedArmorMateria[8];
    u32 exp_to_next_level;
} CharacterRecord;
#pragma pack(pop)

#define CHARACTER_RECORD_ARRAY ((CharacterRecord*)0xDBFD8C)

#endif
