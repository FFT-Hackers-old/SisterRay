#ifndef SR_CONSTANTS_H
#define SR_CONSTANTS_H

#define ELM_FIRE_BIT        (0x0001)
#define ELM_ICE_BIT         (0x0002)
#define ELM_THUNDER_BIT     (0x0004)
#define ELM_EARTH_BIT       (0x0008)
#define ELM_POISON_BIT      (0x0010)
#define ELM_GRAVITY_BIT     (0x0020)
#define ELM_WATER_BIT       (0x0040)
#define ELM_WIND_BIT        (0x0080)
#define ELM_HOLY_BIT        (0x0100)
#define ELM_RESTORE_BIT     (0x0200)
#define ELM_CUT_BIT         (0x0400)
#define ELM_HIT_BIT         (0x0800)
#define ELM_PUNCH_BIT       (0x1000)
#define ELM_SHOOT_BIT       (0x2000)
#define ELM_SHOUT_BIT       (0x4000)
#define ELM_HIDDEN_BIT      (0x8000)

#define ELM_FIRE        (0x0)
#define ELM_ICE         (0x1)
#define ELM_THUNDER     (0x2)
#define ELM_EARTH       (0x3)
#define ELM_POISON      (0x4)
#define ELM_GRAVITY     (0x5)
#define ELM_WATER       (0x6)
#define ELM_WIND        (0x7)
#define ELM_HOLY        (0x8)
#define ELM_RESTORE     (0x9)
#define ELM_CUT         (0xA)
#define ELM_HIT         (0xB)
#define ELM_PUNCH       (0xC)
#define ELM_SHOOT       (0xD)
#define ELM_SHOUT       (0xE)
#define ELM_SHADOW      (0xF)

#define ACT_CLOUD           ((u8)0x10)
#define ACT_BARRET          ((u8)0x11)
#define ACT_TIFA            ((u8)0x12)
#define ACT_AERITH          ((u8)0x13)
#define ACT_RED_XIII        ((u8)0x14)
#define ACT_YUFFIE          ((u8)0x15)
#define ACT_CAIT_SITH       ((u8)0x16)
#define ACT_VINCENT         ((u8)0x17)
#define ACT_CID             ((u8)0x18)

#define ACT_IDX_CLOUD           ((u8)0x0)
#define ACT_IDX_BARRET          ((u8)0x1)
#define ACT_IDX_TIFA            ((u8)0x2)
#define ACT_IDX_AERITH          ((u8)0x3)
#define ACT_IDX_RED_XIII        ((u8)0x4)
#define ACT_IDX_YUFFIE          ((u8)0x5)
#define ACT_IDX_CAIT_SITH       ((u8)0x6)
#define ACT_IDX_VINCENT         ((u8)0x7)
#define ACT_IDX_CID             ((u8)0x8)

#define CMD_ATTACK          ((u8)0x01)
#define CMD_MAGIC           ((u8)0x02)
#define CMD_SUMMON          ((u8)0x03)
#define CMD_ITEM            ((u8)0x04)
#define CMD_STEAL           ((u8)0x05)
#define CMD_SENSE           ((u8)0x06)
#define CMD_COIN            ((u8)0x07)
#define CMD_THROW           ((u8)0x08)
#define CMD_MORPH           ((u8)0x09)
#define CMD_DEATHBLOW       ((u8)0x0A)
#define CMD_MANIPULATE      ((u8)0x0B)
#define CMD_MIME            ((u8)0x0C)
#define CMD_ENEMY_SKILL     ((u8)0x0D)
#define CMD_MUG             ((u8)0x11)
#define CMD_CHANGE          ((u8)0x12)
#define CMD_DEFEND          ((u8)0x13)
#define CMD_LIMIT           ((u8)0x14)
#define CMD_W_MAGIC         ((u8)0x15)
#define CMD_W_SUMMON        ((u8)0x16)
#define CMD_W_ITEM          ((u8)0x17)
#define CMD_SLASH_ALL       ((u8)0x18)
#define CMD_DOUBLE_CUT      ((u8)0x19)
#define CMD_FLASH           ((u8)0x1A)
#define CMD_QUAD_CUT        ((u8)0x1B)
#define CMD_ENEMY_ACTION    ((u8)0x20)
#define CMD_POISONTICK      ((u8)0x23)
#define CMD_TECHNIQUES      ((u8)0x24)
#define STS_DEATH           ((u32)0x1)
#define STS_CRITICAL        ((u32)0x2)
#define STS_SLEEP           ((u32)0x4)
#define STS_POISON          ((u32)0x8)
#define STS_SADNESS         ((u32)0x10)
#define STS_FURY            ((u32)0x20)
#define STS_CONFUSION       ((u32)0x40)
#define STS_SILENCE         ((u32)0x80)
#define STS_HASTE           ((u32)0x100)
#define STS_SLOW            ((u32)0x200)
#define STS_STOP            ((u32)0x400)
#define STS_TOAD            ((u32)0x800)

#define STS_DEATH_IDX           ((u32)0)
#define STS_CRITICAL_IDX        ((u32)1)
#define STS_SLEEP_IDX           ((u32)2)
#define STS_POISON_IDX          ((u32)3)
#define STS_SADNESS_IDX         ((u32)4)
#define STS_FURY_IDX            ((u32)5)
#define STS_CONFUSION_IDX       ((u32)6)
#define STS_SILENCE_IDX         ((u32)7)
#define STS_HASTE_IDX           ((u32)8)
#define STS_SLOW_IDX            ((u32)9)
#define STS_STOP_IDX            ((u32)10)
#define STS_TOAD_IDX            ((u32)11)
#define STS_MINI_IDX            ((u32)12)
#define STS_SLOWNUMB_IDX        ((u32)13)
#define STS_STONE_IDX           ((u32)14)
#define STS_REGEN_IDX           ((u32)15)
#define STS_BARRIER_IDX         ((u32)16)
#define STS_MBARRIER_IDX        ((u32)17)
#define STS_REFLECT_IDX         ((u32)18)
#define STS_DUAL_IDX            ((u32)19)
#define STS_SHIELD_IDX          ((u32)20)
#define STS_DOOM_IDX            ((u32)21)
#define STS_MANIP_IDX           ((u32)22)
#define STS_BERSERK_IDX        ((u32)23)
#define STS_PEERLESS_IDX        ((u32)24)
#define STS_PARALYSIS_IDX       ((u32)25)
#define STS_BLIND_IDX           ((u32)26)
#define STS_DRAIN_IDX           ((u32)27)
#define STS_ZOMBIE_IDX          ((u32)28)
#define STS_RESIST_IDX          ((u32)29)
#define STS_LUCKYGIRL_IDX       ((u32)30)
#define STS_IMPRISON_IDX        ((u32)31)

#define SR_WIDGET_TEXT_TYPE   ((u16)0x00)
#define SR_WIDGET_NUMBER_TYPE ((u16)0x01)
#define SR_WIDGET_BOX_TYPE    ((u16)0x02)
#define SR_WIDGET_ARROW_TYPE  ((u16)0x03)

#define TGT_FLAG_ENABLE_SELECT   ((u8)0x01)
#define TGT_FLAG_START_ENEMIES   ((u8)0x02)
#define TGT_FLAG_START_MULTIPLE  ((u8)0x04)
#define TGT_FLAG_TOGGLE_MULTIPLE ((u8)0x08)
#define TGT_FLAG_ONE_ROW         ((u8)0x10)
#define TGT_SHORT_RANGE          ((u8)0x20)
#define TGT_FLAG_ALL_ROWS        ((u8)0x40)
#define TGT_FLAG_RANDOM          ((u8)0x80)


#define TGT_TYPE_ALLIES          ((u8)0x01)
#define TGT_TYPE_ENEMIES         ((u8)0x02)
#define TGT_TYPE_ALLIES_ALL      ((u8)0x05)
#define TGT_TYPE_ENEMIES_ALL     ((u8)0x06)

#define BATTLE_INACTIVE          33
#define BATTLE_TARGETING_STATE   0
#define BATTLE_LIMIT_STATE       20
#define BATTLE_CMD_STATE         1
#define BATTLE_SUMMON_STATE      7
#define BATTLE_MAGIC_STATE       6
#define BATTLE_ITEM_STATE        5
#define BATTLE_ESKILL_STATE      4
#define BATTLE_MANIP_STATE       19
#define BATTLE_PHS_STATE         34
#define BATTLE_MOVE_STATE        35
#define BATTLE_TECHNIQUE_STATE   36

#define SLOT_LEFT_LINKED           3
#define SLOT_RIGHT_LINKED          2
#define SLOT_LEFT_LINKED_2         7
#define SLOT_RIGHT_LINKED_2        6

enum GameModule {
    NO_MODULE,
    FIELD,
    BATTLE,
    WORLD_MAP,
    MODULE_4,
    MENU,
};
#endif
