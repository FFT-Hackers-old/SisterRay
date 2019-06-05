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

#define SR_WIDGET_TEXT_TYPE   ((u16)0x00)
#define SR_WIDGET_NUMBER_TYPE ((u16)0x01)
#define SR_WIDGET_BOX_TYPE    ((u16)0x02)
#define SR_WIDGET_ARROW_TYPE  ((u16)0x03)

#endif
