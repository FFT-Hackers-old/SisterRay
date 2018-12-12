//Define relevant constants for accessing specific parts of the exe*//

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;

const u32 FORMULA_TABLE_LOCATION = 0x8FF1F8;

const u32 PHYSICAL_DAMAGE_FORMULA = 0x5DE5DF;
const u32 MAGICAL_DAMAGE_FORMULA = 0x5DE9B8;
const u32 CURRENT_HP_GRAVITY = 0x5DEA6D;
const u32 MAX_HP_GRAVITY = 0x5DEAF7;
const u32 CURE_FORMULA = 0x5DEB81;
const u32 CONSTANT_20 = 0x5DEBE5;
const u32 CONSTANT_16 = 0x5DEC0A;
const u32 RECOVERY_FORMULA = 0x5DEC52;
const u32 THROW_FORMULA = 0x5DEC8A;
const u32 COIN_FORMULA = 0x5DECAA;

const DamageCalcStruct* DamageContextPtr = 0x99CE0C;

const u32 ACTOR_ARRAY_START = 0x9AB0DC;
int ACTOR_ARRAY_SIZE = 10;


#if defined(__GNUC__)
# define PACKED  __attribute__ ((packed))
#else
# define PACKED
#endif

//This struct has size 260h and is referenced by the main context ptr
typedef struct {

} PACKED DamageCalcStruct;

//An array of size ACTOR_ARRAY_SIZE w/ elements of size 0x68 exists at ARRAY_ACTOR_START
typedef struct {
    u32     statusMasks;
    u32     actorFlags;
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
    u16      lastTargets;
    u16     prevAttackerMask;
    u16     prevPhysAttackerMask;
    u16     prevMagAttackerMask;
    u16     Defense;
    u16     MDefense;
    u16     formationID;
    u16     absorbedElementsMask;
    u16     currentMP;
    u16     maxMP;
    u32     currentHP;
    u32     maxHP;
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

} PACKED ActorBattleVars;
