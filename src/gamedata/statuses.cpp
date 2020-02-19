#include "statuses.h"
#include "status_names.h"
#include "../impl.h"
#include "../party/stat_names.h"

SrStatusRegistry::SrStatusRegistry(bool useResistances) {
    SrStatus death{ EncodedString::from_unicode(StatusNames::DEATH_NAME.c_str()), true, 0, StatNames::DEATH_PEN, StatNames::DEATH_RES };
    addElement(StatusNames::DEATH, death);
    SrStatus critical{ EncodedString::from_unicode(StatusNames::CRITICAL_NAME.c_str()), true, 1, StatNames::CRITICAL_PEN, StatNames::CRITICAL_RES };
    addElement(StatusNames::CRITICAL, critical);
    SrStatus sleep{ EncodedString::from_unicode(StatusNames::SLEEP_NAME.c_str()), true, 2, StatNames::SLEEP_PEN, StatNames::SLEEP_RES };
    addElement(StatusNames::SLEEP, sleep);
    SrStatus poison{ EncodedString::from_unicode(StatusNames::PSN_STS_NAME.c_str()), true, 3, StatNames::POISON_PEN_STS, StatNames::POISON_RES_STS };
    addElement(StatusNames::POISON, poison);
    SrStatus sadness{ EncodedString::from_unicode(StatusNames::SADNESS_NAME.c_str()), true, 4, StatNames::SADNESS_PEN, StatNames::SADNESS_RES };
    sadness.neutralizeOnInflict.push_back(StatusNames::FURY);
    addElement(StatusNames::SADNESS, sadness);
    SrStatus fury{ EncodedString::from_unicode(StatusNames::FURY_NAME.c_str()), true, 5, StatNames::FURY_PEN, StatNames::FURY_RES };
    fury.neutralizeOnInflict.push_back(StatusNames::SADNESS);
    addElement(StatusNames::FURY, fury);
    SrStatus confusion{ EncodedString::from_unicode(StatusNames::CONFUSION_NAME.c_str()), true, 6, StatNames::CONFUSION_PEN, StatNames::CONFUSION_RES };
    addElement(StatusNames::CONFUSION, confusion);
    SrStatus silence{ EncodedString::from_unicode(StatusNames::SILENCE_NAME.c_str()), true, 7, StatNames::SILENCE_PEN, StatNames::SILENCE_RES };
    addElement(StatusNames::SILENCE, silence);
    SrStatus haste{ EncodedString::from_unicode(StatusNames::HASTE_NAME.c_str()), true, 8, StatNames::HASTE_PEN, StatNames::HASTE_RES };
    haste.neutralizeOnInflict.push_back(StatusNames::SLOW);
    addElement(StatusNames::HASTE, haste);
    SrStatus slow{ EncodedString::from_unicode(StatusNames::SLOW_NAME.c_str()), true, 9, StatNames::SLOW_PEN, StatNames::SLOW_RES };
    slow.neutralizeOnInflict.push_back(StatusNames::HASTE);
    addElement(StatusNames::SLOW, slow);
    SrStatus sleep{ EncodedString::from_unicode(StatusNames::SLEEP_NAME.c_str()), true, 10, StatNames::STOP_PEN, StatNames::STOP_RES };
    addElement(StatusNames::SLEEP, sleep);
    SrStatus frog{ EncodedString::from_unicode(StatusNames::FROG_NAME.c_str()), true, 11, StatNames::FROG_PEN, StatNames::FROG_RES };
    addElement(StatusNames::FROG, frog);
    SrStatus mini{ EncodedString::from_unicode(StatusNames::MINI_NAME.c_str()), true, 12, StatNames::SMALL_PEN, StatNames::SMALL_RES };
    addElement(StatusNames::SMALL, mini);
    SrStatus slownumb{ EncodedString::from_unicode(StatusNames::SLOWNUMB_NAME.c_str()), true, 13, StatNames::SLOWNUMB_PEN, StatNames::SLOWNUMB_RES };
    addElement(StatusNames::SLOWNUMB, slownumb);
    SrStatus petrify{ EncodedString::from_unicode(StatusNames::PETRIFY_NAME.c_str()), true, 14, StatNames::PETRIFY_PEN, StatNames::PETRIFY_RES };
    addElement(StatusNames::PETRIFY, confusion);
    SrStatus regen{ EncodedString::from_unicode(StatusNames::REGEN_NAME.c_str()), true, 15, StatNames::REGEN_PEN, StatNames::REGEN_RES };
    addElement(StatusNames::REGEN, regen);
    SrStatus barrier{ EncodedString::from_unicode(StatusNames::BARRIER_NAME.c_str()), true, 16, StatNames::BARRIER_PEN, StatNames::BARRIER_RES };
    addElement(StatusNames::BARRIER, barrier);
    SrStatus mbarrier{ EncodedString::from_unicode(StatusNames::MBARRIER_NAME.c_str()), true, 17, StatNames::MBARRIER_PEN, StatNames::MBARRIER_RES };
    addElement(StatusNames::MBARRIER, mbarrier);
    SrStatus reflect{ EncodedString::from_unicode(StatusNames::REFLECT_NAME.c_str()), true, 18, StatNames::REFLECT_PEN, StatNames::REFLECT_RES };
    addElement(StatusNames::REFLECT, reflect);
    SrStatus dual{ EncodedString::from_unicode(StatusNames::DUAL_NAME.c_str()), true, 19, StatNames::DUAL_PEN, StatNames::DUAL_RES };
    addElement(StatusNames::DUAL, dual);
    SrStatus shield{ EncodedString::from_unicode(StatusNames::SHIELD_NAME.c_str()), true, 20, StatNames::SHIELD_PEN, StatNames::SHIELD_RES };
    addElement(StatusNames::SHIELD, shield);
    SrStatus doom{ EncodedString::from_unicode(StatusNames::DOOM_NAME.c_str()), true, 21, StatNames::DOOM_PEN, StatNames::DOOM_RES };
    addElement(StatusNames::DOOM, doom);
    SrStatus manipulate{ EncodedString::from_unicode(StatusNames::MANIPULATE_NAME.c_str()), true, 22, StatNames::MANIPULATE_PEN, StatNames::MANIPULATE_RES };
    addElement(StatusNames::MANIPULATE, manipulate);
    SrStatus berserk{ EncodedString::from_unicode(StatusNames::BERSERK_NAME.c_str()), true, 23, StatNames::BERSERK_PEN, StatNames::BERSERK_RES };
    addElement(StatusNames::BERSERK, berserk);
    SrStatus peerless{ EncodedString::from_unicode(StatusNames::PEERLESS_NAME.c_str()), true, 24, StatNames::PEERLESS_PEN, StatNames::PEERLESS_RES };
    addElement(StatusNames::PEERLESS, peerless);
    SrStatus paralysis{ EncodedString::from_unicode(StatusNames::PARALYSIS_NAME.c_str()), true, 25, StatNames::PARALYSIS_PEN, StatNames::PARALYSIS_RES };
    addElement(StatusNames::PARALYSIS, paralysis);
    SrStatus blind{ EncodedString::from_unicode(StatusNames::BLIND_NAME.c_str()), true, 26, StatNames::BLIND_PEN, StatNames::BLIND_RES };
    addElement(StatusNames::BLIND, blind);
    SrStatus drain{ EncodedString::from_unicode(StatusNames::DRAIN_NAME.c_str()), true, 27, StatNames::DRAIN_PEN, StatNames::DRAIN_RES };
    addElement(StatusNames::DRAIN, drain);
    SrStatus zombie{ EncodedString::from_unicode(StatusNames::ZOMBIE_NAME.c_str()), true, 28, StatNames::ZOMBIE_PEN, StatNames::ZOMBIE_RES };
    addElement(StatusNames::ZOMBIE, zombie);
    SrStatus resist{ EncodedString::from_unicode(StatusNames::RESIST_NAME.c_str()), true, 29, StatNames::RESIST_PEN, StatNames::RESIST_RES };
    addElement(StatusNames::RESIST, resist);
    SrStatus luckygirl{ EncodedString::from_unicode(StatusNames::LUCKYGIRL_NAME.c_str()), true, 30, StatNames::LUCKYGIRL_PEN, StatNames::LUCKYGIRL_RES };
    addElement(StatusNames::LUCKYGIRL, luckygirl);
    SrStatus imprison{ EncodedString::from_unicode(StatusNames::IMPRISON_NAME.c_str()), true, 31, StatNames::IMPRISON_PEN, StatNames::IMPRISON_RES };
    addElement(StatusNames::IMPRISON, imprison);
}

void initStatuses() {
    gContext.statuses = SrStatusRegistry(true);
    srLogWrite("kernel.bin: Loaded %lu statuses", (unsigned long)gContext.statuses.resourceCount());
}


std::string getStatusIDFromIndex(u8 statusIdx) {
    switch (statusIdx) {
    case STS_DEATH_IDX: {
        return StatusNames::DEATH;
    }
    case STS_CRITICAL_IDX: {
        return StatusNames::CRITICAL;
    }
    case STS_SLEEP_IDX: {
        return StatusNames::SLEEP;
    }
    case STS_POISON_IDX: {
        return StatusNames::POISON;
    }
    case STS_SADNESS_IDX: {
        return StatusNames::SADNESS;
    }
    case STS_FURY_IDX: {
        return StatusNames::FURY;
    }
    case STS_CONFUSION_IDX: {
        return StatusNames::CONFUSION;
    }
    case STS_SILENCE_IDX: {
        return StatusNames::SILENCE;
    }
    case STS_SLOW_IDX: {
        return StatusNames::SLOW;
    }
    case STS_HASTE_IDX: {
        return StatusNames::HASTE;
    }
    case STS_STOP_IDX: {
        return StatusNames::STOP;
    }
    case STS_TOAD_IDX: {
        return StatusNames::FROG;
    }
    case STS_MINI_IDX: {
        return StatusNames::SMALL;
    }
    case STS_SLOWNUMB_IDX: {
        return StatusNames::SLOWNUMB;
    }
    case STS_STONE_IDX: {
        return StatusNames::PETRIFY;
    }
    case STS_REGEN_IDX: {
        return StatusNames::REGEN;
    }
    case STS_BARRIER_IDX: {
        return StatusNames::BARRIER;
    }
    case STS_MBARRIER_IDX: {
        return StatusNames::MBARRIER;
    }
    case STS_REFLECT_IDX: {
        return StatusNames::REFLECT;
    }
    case STS_DUAL_IDX: {
        return StatusNames::DUAL;
    }
    case STS_SHIELD_IDX: {
        return StatusNames::SHIELD;
    }
    case STS_DOOM_IDX: {
        return StatusNames::DOOM;
    }
    case STS_MANIP_IDX: {
        return StatusNames::MANIPULATE;
    }
    case STS_BERSERK_IDX: {
        return StatusNames::BERSERK;
    }
    case STS_PEERLESS_IDX: {
        return StatusNames::PEERLESS;
    }
    case STS_BLIND_IDX: {
        return StatusNames::BLIND;
    }
    case STS_DRAIN_IDX: {
        return StatusNames::DRAIN;
    }
    case STS_ZOMBIE_IDX: {
        return StatusNames::ZOMBIE;
    }
    case STS_RESIST_IDX: {
        return StatusNames::RESIST;
    }
    case STS_LUCKYGIRL_IDX: {
        return StatusNames::LUCKYGIRL;
    }
    case STS_IMPRISON_IDX: {
        return StatusNames::IMPRISON;
    }
    case STS_SILENCE_IDX: {
        return StatusNames::SILENCE;
    }
    }
}
