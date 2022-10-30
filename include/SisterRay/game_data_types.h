#ifndef GAME_DATA_TYPES_H
#define GAME_DATA_TYPES_H

#include <SisterRay/types.h>

typedef void(*SRPFNSPELLEFFECTCALLBACK)(u32, u32);
typedef void(*SRPFNSPELLLOADERCALLBACK)();


typedef struct SrWeapon_ SrWeapon;
typedef struct SrArmor_ SrArmor;
typedef struct SrCommand_ SrCommand;
typedef struct SrAttack_ SrAttack;
typedef struct SrBattleStat_ SrBattleStat;
typedef struct StatFormulaCtx_ StatFormulaCtx;

typedef struct {
    SRPFNSPELLLOADERCALLBACK loadCallback;
    SRPFNSPELLEFFECTCALLBACK dispatchCallback;
} SpellEffect;

typedef enum {
    MAGIC,
    SUMMON,
    ENEMY_SKILL,
    ITEM,
    THROW,
    COIN,
    LIMIT,
    ENEMY_ATTACK,
    NO_EFFECT
} SrAnimationType;

typedef enum {
    SR_INVALID_GEAR_TYPE,
    SR_GEAR_WEAPON,
    SR_GEAR_ARMOR,
    SR_GEAR_ACCESSORY
} SrGameGearType;

#pragma pack(push, 1)
typedef struct {
    u8 commandMenuID;
    u8 targetingFlags;
    u8 align[2];
    u16 singleCameraID;
    u16 multipleCameraID;
} CommandData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 animationScriptIndex;
    u8 damageCalculationByte; //We can probably modularize this
    u16 miscCommandFlags;
    u8 hasActions;
    SrAnimationType animationType;
    u8 animationEffectID;
    u8 useOverride;
    SpellEffect override;
    u8 useMulti;
    SpellEffect spellEffectMulti;
} AuxCommandData;
#pragma pack(pop)

typedef struct {
    CommandData baseData;
    AuxCommandData auxData;
    const char* name;
    const char* description;
} SrCommandData;

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


typedef struct {
    AttackData baseData;
    const char* name;
    const char* description;
} SrActionData;

#pragma pack(push, 1)
typedef struct {
    u8      unknown[8];
    u16     cameraMovement;
    u16     restrictionMask;
    u8      targetingFlags;
    u8      animationEffectID;
    u8      damageFormula;
    u8      itemPower;
    u8      restoreTypes;
    u8      statusInflictType;
    u8      additionalEffectID;
    u8      additionalEffectModifier;
    u32     statusMask;
    u16     elementMask;
    u16     specialAttackFlags;
} ItemData;
#pragma pack(pop)

typedef struct {
    u16 hp_heal_amount;
    u16 mp_heal_amount;
    u16 stat_to_boost;
    u16 characterRestrictionMask; //Used to make certain items usable only by certain characters
    u8 hp_heal_percent;
    u8 mp_heal_percent;
    bool can_revive;
    bool target_all;
    bool requires_target;
} ConsumableUseData;

typedef struct {
    ItemData baseData;
    ConsumableUseData useData;
    const char* itemName;
    const char* itemDesc;
} SrConsumable;

#pragma pack(push, 1)
typedef struct {
    u8      targetFlags;
    u8      attackEffectID;
    u8      damageCalculation;
    u8      unused_byte;
    u8      weaponStrength;
    u8      status_attack;
    u8      materia_growth;
    u8      criticalRate;
    u8      weaponHitRate;
    u8      weapon_model;
    u8      padding;
    u8      soundAccessMask;
    u16     cameraMovement;
    u16     equipMask;
    u16     attackElementMask;
    u16     unknown;
    u8      stats_to_boost[4];
    u8      stat_boost_amounts[4];
    u8      materiaSlots[8];
    u8      normalSoundID;
    u8      critSoundID;
    u8      missSoundID;
    u8      impactEffectID;
    u16     specialAttackFlags;
    u16     restrictionMask;
} GameWeapon;
#pragma pack(pop)

typedef struct {
    GameWeapon baseData;
    const char* weaponName;
    const char* weaponDesc;
} SrWeaponData;

#pragma pack(push, 1)
typedef struct {
    u8      unknown;
    u8      elementDamageType;
    u8      defense;
    u8      magicDefense;
    u8      evade;
    u8      magicEvade;
    u8      status_defense;
    u16     unknown2;
    u8      materiaSlots[8];
    u8      materia_growth;
    u16     equipMask;
    u16     elemental_defense_mask;
    u16     unknown3;
    u8      stats_to_boost[4];
    u8      stat_boost_amounts[4];
    u16     restrictionMask;
    u16     unknown4;
} GameArmor;
#pragma pack(pop)

typedef struct {
    GameArmor baseData;
    const char* armorName;
    const char* armorDesc;
} SrArmorData;

#pragma pack(push, 1)
typedef struct {
    u8      stats_to_boost[2];
    u8      stat_boost_amounts[2];
    u8      elemental_interaction;
    u8      special_effect; /*We will hook in custom effects here*/
    u16     elements_mask;
    u32     status_defense_mask;
    u16     equipMask;
    u16     restrictionMask;
} GameAccessory;
#pragma pack(pop)

typedef struct {
    GameAccessory baseData;
    const char* accessoryName;
    const char* accessoryDesc;
} SrAccessoryData;

#pragma pack(push, 1)
typedef struct {
    u16     apLevel[4];
    u8      equipEffect;
    u8      statusEffects[3];
    u8      elementType;
    u8      type;
    u8      data[6];
} MateriaData;
#pragma pack(pop)

typedef struct {
    bool doesExpire;
    bool doesStack;
    u8 stacks;
    bool allowMultiple;
    bool persistAfterBattle;
} SrStatusBehavior;

typedef struct {
    u8 canLink;
} AuxMateriaData;

typedef struct {
    MateriaData baseData;
    AuxMateriaData auxData;
    const char* materiaName;
    const char* materiaDesc;
} SrMateriaData;

typedef struct {
    u16 materiaID;
    u32 materiaAP;
} MateriaInventoryEntry;

#endif // !GAME_DATA_TYPES_H
