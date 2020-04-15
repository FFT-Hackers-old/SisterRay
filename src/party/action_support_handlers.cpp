#include "action_support_handlers.h"
#include "../impl.h"

/*All Handlers*/
void allT9S1(const ApplySupportEvent* event) {
}

/*Counter handlers*/
void counterT6S1(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 84)
        return;

    for (auto dataIdx = 4; dataIdx >= 0; dataIdx--) {
        auto commandIdx = event->pairedData.data[dataIdx];
        if (commandIdx == 0xFF) {
            continue;
        }
        if (dataIdx < event->pairedMateriaLevel) {
            addAutoAction(
                event->characterIdx,
                COUNTER_ACTION,
                commandIdx,
                0xFF,
                event->SupportMateriaData.data[event->supportMateriaLevel],
                event->supportMateriaLevel
            );
            return;
        }
    }
}


/*Magic Counter handlers*/
void mCounterT9S1(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 85)
        return;

    for (auto dataIdx = 4; dataIdx >= 0; dataIdx--) {
        auto spellIdx = event->pairedData.data[dataIdx];
        if (spellIdx == 0xFF) {
            continue;
        }
        if (dataIdx < event->pairedMateriaLevel) {
            addAutoAction(
                event->characterIdx,
                COUNTER_ACTION,
                CMD_MAGIC,
                spellIdx,
                event->SupportMateriaData.data[event->supportMateriaLevel],
                event->supportMateriaLevel
            );
            return;
        }
    }
}

void mCounterT11S3(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 85)
        return;

    auto spellIdx = event->pairedData.data[0];
    if (spellIdx == 0xFF) {
        return;
    }
    addAutoAction(
        event->characterIdx,
        COUNTER_ACTION,
        CMD_SUMMON,
        spellIdx,
        event->SupportMateriaData.data[event->supportMateriaLevel],
        event->pairedData.data[event->pairedMateriaLevel]
    );
}

/*SNEAK ATTACK*/
void sAttackT9S1(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 86)
        return;

    for (auto dataIdx = 4; dataIdx >= 0; dataIdx--) {
        auto spellIdx = event->pairedData.data[dataIdx];
        if (spellIdx == 0xFF) {
            continue;
        }
        if (dataIdx < event->pairedMateriaLevel) {
            addAutoAction(
                event->characterIdx,
                SNEAK_ATTACK,
                CMD_MAGIC,
                spellIdx,
                event->SupportMateriaData.data[event->supportMateriaLevel],
                event->supportMateriaLevel
            );
            return;
        }
    }
}


void sAttackT11S3(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 86)
        return;

    auto spellIdx = event->pairedData.data[0];
    if (spellIdx == 0xFF) {
        return;
    }
    addAutoAction(
        event->characterIdx,
        SNEAK_ATTACK,
        CMD_SUMMON,
        spellIdx,
        event->SupportMateriaData.data[event->supportMateriaLevel],
        event->pairedData.data[event->pairedMateriaLevel]
    );
}

/*FINAL ATTACK*/
void fAttackT9S1(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 87)
        return;

    for (auto dataIdx = 4; dataIdx >= 0; dataIdx--) {
        auto spellIdx = event->pairedData.data[dataIdx];
        if (spellIdx == 0xFF) {
            continue;
        }
        if (dataIdx < event->pairedMateriaLevel) {
            addAutoAction(
                event->characterIdx,
                FINAL_ATTACK,
                CMD_MAGIC,
                spellIdx,
                event->SupportMateriaData.data[event->supportMateriaLevel],
                event->supportMateriaLevel
            );
            return;
        }
    }
}


void fAttackT11S3(const ApplySupportEvent* event) {
    if (event->SupportMateriaData.data[0] != 87)
        return;

    auto spellIdx = event->pairedData.data[0];
    if (spellIdx == 0xFF) {
        return;
    }
    addAutoAction(
        event->characterIdx,
        FINAL_ATTACK,
        CMD_SUMMON,
        spellIdx,
        event->SupportMateriaData.data[event->supportMateriaLevel],
        event->pairedData.data[event->pairedMateriaLevel]
    );
}
