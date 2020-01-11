#ifndef SR_PARTY_H
#define SR_PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/game_data_types.h>

#pragma pack(push, 1)
typedef struct {
    u8 field_0;
    u8 attackerID;
    u8 commandIdx;
    u16 relAttackIdx;
    u16 targetMask;
    u8 pad;
} MimeAction;
#pragma pack(pop)

#define G_MIME_ACTIONS ((MimeAction*)(0x9A9ADD))

#pragma pack(push, 1)
typedef struct {
    u8 commandID;
    u8 cursorCommandType;
    u8 targetingData;
    u8 commandFlags;
    u8 allCount;
    u8 supportMatFlags;
} EnabledCommand;
#pragma pack(pop)

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
    u16 atbTimer; //0x18
    u16 limitGuage;
    u16 barrierGauge; //0x1C
    u16 mBarrierGauge; //0x1E
    u8 unkbyte; //0x20
    u8 commandColumns; //0x21
    u8 unknownDiviosr; //0x22
    u8 actorFlags; //0x23
    u8 unknown24bitInts[24]; //0x24
    u16 attackElementsMask; //0x3C
    u16 halvedElementsMask; //003E
    u16 nulledElementsMask; //0x40
    u16 absorbeElementsMask; //0x42
    u32 attackStatusesMask; //0x44
    u32 immuneStatusesMask; //0x48
    EnabledCommand enabledCommandArray[16]; //0x4C
    u8 enabledLimitBytes[8]; //0xAC
    AttackData enabledLimitData[3]; //0xB4
    EnabledSpell enabledMagicsData[54]; //0x108
    EnabledSpell unusedMagics[2]; //0x2B8
    EnabledSpell enabledSummons[16]; //0x2C8
    EnabledSpell enabledEnemySkills[24]; //0x348
    WeaponData weaponData;
    u32 unkthing2;
    u32 unkthing;
    u8 charGilBonus;
    u8 encounterRate;
    u8 chocoboChance;
    u8 preEmptiveChance;
} PartyMember;
#pragma pack(pop)

#define PARTY_STRUCT_ARRAY ((PartyMember*)0xDBA498)

#pragma pack(push, 1)
typedef struct {
    u8 characterID;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    u8 deathType;
    u8 field_5;
    u16 field_6;
    u32 field_8;
    u32 field_C;
}UnkBattleStruct;
#pragma pack(pop)


#define UNK_ACTOR_STRUCT_ARRAY ((UnkBattleStruct*)0x9A87F4)

#endif // !SR_PARTY_H
