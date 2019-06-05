#ifndef SISTER_RAY_GAME_STRUCTS_H
#define SISTER_RAY_GAME_STRUCTS_H

#include <SisterRay/types.h>

typedef struct {
    u8 entryPriority;
    u8 entryOffset;
    u8 attackerActorID;
    u8 actionCommandIndex;
    u16 actionAttackIndex;
    u16 actionTargetmask;
} BattleQueueEntry;

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
    u32 damageCalcMask; //0x78
    u32 unkTHATCRASHEDTHEGAME; // 0x7C
    u32 addStatusMask; //0x80
    u32 rmStatusMask; //0x84
    u32 toggleStatusMask; //0x88
    u32 inflictStatusChance; //0x8C
    u32 miscActionflags; //0x90
    u32 finalTargetMask; //0x94
    u32 sceneAbilityIndex; //0x98
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
    u32 targetReactionAnimation; //0xCC
    u32 unkDWord2; //0xD0
    u32 unkDWord3; //0xD4
    u32 throwFormulaPower; //0xD8
    u32 displayString; //0xDC
    u32 incOnDamageDealt; //0xE0
    u32 unkDWord4; //0xE4
    u32 MPTurboBoost; //0xE8
    u32 usedTargetMask; //0xEC
    u32 addedCutMPHPAbsorbByte; //0xF0
    u32 currentUnitIDTempt; //0xF4
    u32 unkDWord5; //0xF8
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
    u16 vTimerIncrement; //0x00
    u16 yurnTimerIncrement; //0x02
    u16 charATBValue; //0x04
    u16 unk[2];       //0x06
    u16 CTimerIncrement; //0xA
    u8 unk1;             //0x0B
    u8 unk2;             //0x0C
    u8 activeCommandsMask; //0x0D
    u8 unk3;               //0x0F
    u8 unk4;               //0x10
    u8 align[3];           //0x11
    u8 unk5;               //0x15
    u8 unk6;               //0x16
    u8 PoisonTick;         //0x17
    u8 align2[9];           //0x20
    u8 unk7;               //0x21
    u8 align3[7];           //0x28
    u8 unk8;               //0x29
    u8 unk9;               //0x2A
    u8 unk10;              //0x2B
    u32 unkdword1;         //0x2C
    u32 unkdword2;         //0x30
    u32 unkdowrd3;         //0x34
    u32 unkdword4;         //0x38
    u16 currentHP;          //0x3C
    u16 currentMP;          //0x3E
    u8 padding[4];         //0x40
} ActorTimerData;
#pragma pack(pop)

#define gActorTimerBlock ((ActorTimerData*)(0x9A8B10))

/*Should  have size 0x1AEC*/
#pragma pack(push, 1)
typedef struct {
	u16 ActorID; //BE1170
	u8 padding1[6]; //BE1171
	u16 AnimationData; //BE1178
	u16 commandAnimID; //BE117A
	u8 padding2[30]; //BE117C
	u8 AnimationEffect; //BE119A
	u8 commandID; //BE119B
	u8 padding3[0x1AC0];
} BigAnimBlock;
#pragma pack(pop)

#define gBigAnimBlock       ((BigAnimBlock*)0xBE1170)

#pragma pack(push, 1)
typedef struct {
    u8 commandID;
    u8 cursorCommandType;
    u8 targetingData;
    u8 commandFlags;
    u8 allCount;
    u8 HPMPAbsorbFags;
} EnabledCommandStruct;
#pragma pack(pop)

/*This is the structure of attack data*/
typedef struct {
    u8 abilityHitRate; //0x00
    u8 impactEffectID; //0x01
    u8 targetReactionID; //0x02
    u8 unkbyte;          //0x03
    u16 MPCost;          //0x04
    u16 impactSoundID;   //0x06
    u16 cameraMovementSingle;   //0x08
    u16 cameraMovementMultiple;  //0x0A
    u8 targetingFlags;           //0x0C
    u8 animationEffectID;        //0x0D
    u8 damageFormula;            //0x0E
    u8 attackPower;              //0x0F
    u8 restoreTypes;             //0x10
    u8 statusInflictType;         //0x11
    u8 additionalEffect;         //0x12
    u8 additionalEffectModifier;  //0x13
    u32 statusMask;               //0x14
    u16 elementMask;              //0x18
    u16 specialAttackFlags;       //0x1A
} AttackData;

/*This contains a bit vector of flags for spells*/
#pragma pack(push, 1)
typedef struct {
    u8 magicIndex;
    u8 mpCost;
    u8 allCount; //used as summon count for summons
    u8 quadEnabled;
    u8 quadCount;
    u8 targetData;
    u8 propertiesMask;
    u8 supportEffectsMask; //HP Absorb, etc
} EnabledSpell;
#pragma pack(pop)


typedef enum {
    AUTOACT_NO_ACTION,
    SNEAK_ATTACK,
    COUNTER_ACTION,
    FINAL_ATTACK,
} AutoActionType;

typedef struct {
    AutoActionType dispatchType;
    u8 commandIndex;
    u16 actionIndex;
    u8 activationChance; //as a %
    u8 counterCount; //based on the level of the linked counter, 0xFF for unlimited counters
} SrAutoAction;

#pragma pack(push,1)
typedef struct {
    u32 statToIncrease;
    u32 increaseValue;
} statIncrease;
#pragma pack(pop)

#pragma pack(push, 1)
/*Three of these are maintained for each active party member, they have size 0x440*/
typedef struct {
    u8 characterID; //0x00
    u8 coverChance; //0x01
    u8 strength; //0x02
    u8 vitality; //0x03
    u8 magic; //0x04
    u8 spirit; //0x05
    u8 speed; //0x06
    u8 luck; //0x07
    u16 physAttack; //0x08
    u16 physDefense; //0x0A
    u16 magAttack; //0x0C
    u16 magDefense; //0x0E
    u16 currentHP; //0x10
    u16 maxHP; //0x12
    u16 currentMP; //0x14
    u16 maxMP; //0x16
    u32 timer; //0x18
    u16 counterActionIndex; //0x1C
    u16 counterChance; //0x1E
    u8 unkbyte; //0x20
    u8 commandColumns; //0x21
    u8 unknownDiviosr; //0x22
    u8 commandRows; //0x23
    u8 unknown24bitInts[24]; //0x24
    u16 attackElementsMask; //0x3C
    u16 halvedElementsMask; //003E
    u16 nulledElementsMask; //0x40
    u16 absorbeElementsMask; //0x42
    u32 attackStatusesMask; //0x44
    u32 immuneStatusesMask; //0x48
    EnabledCommandStruct enabledCommandArray[16]; //0x4C
    u8 enabledLimitBytes[8]; //0xAC
    AttackData enabledLimitData[3]; //0xB4
    EnabledSpell enabledMagicsData[54]; //0x108
    EnabledSpell unusedMagics[2]; //0x2B8
    EnabledSpell enabledSummons[16]; //0x2C8
    EnabledSpell enabledEnemySkills[24]; //0x348
    u8 weaponData[5]; //0x408
    u16 weaponStatus; //0x40D
    u8 lepad;
    u8 weaponAccuracy; //0x410
    u8 paddin[7];
    u32 additionalAttackElements;
    statIncrease statsToIncrease[4];
    u8 charGilBonus;
    u8 encounterRate;
    u8 chocoboChance;
    u8 preEmptiveChance;
} ActivePartyMemberStruct;
#pragma pack(pop)

#define PARTY_STRUCT_ARRAY ((ActivePartyMemberStruct*)0xDBA498)

#pragma pack(push, 1)
typedef struct {
    u8 characterID; //0x00
    u8 unkByte1;    //0x01
    u8 unkByte2;    //0x02
    u8 unkByte3;    //0x03
    u8 unkByte4;    //0x04
    u8 align;       //0x05
    u16 unkWrd1;    //0x06
    u32 unkDWrd1;   //0x08
    u32 unkDwrd2;   //0x0C
} UnkPartyStruct;
#pragma pack(pop)

#define gUnkPartyDataArray ((UnkPartyStruct*)0x9A87F4)

#pragma pack(push, 1)
typedef struct {
    u8 character_id;
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
    u8 character_name[12];
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
} characterRecord;
#pragma pack(pop)

#define CHARACTER_RECORD_ARRAY ((characterRecord*)0xDBFD8C)

#pragma pack(push, 1)
typedef struct {
    u32 relativeColumnIndex;
    u32 relativeRowIndex;
    u32 viewColumnBound;
    u32 viewRowBound;
    u32 baseColumnIndex;
    u32 baseRowIndex;
    u32 maxColumnBound;
    u32 maxRowBound;
    u32 ninth_dword;
    u32 tenth_dword;
    u32 eleventh_dword;
    u32 twelth_dword;
    u32 thirteenth_dword;
    u32 fourteenh_dword;
} CursorContext;
#pragma pack(pop)
#endif
