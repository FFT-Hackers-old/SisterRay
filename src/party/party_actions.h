#ifndef PARTY_ACTIONS_H
#define PARTY_ACTIONS_H

#include <SisterRay/SisterRay.h>

template<size_t N>
void clearActionArray(std::array<EnabledSpell, N>& spellArray) {
    const EnabledSpell nullSpell = { 0xFF, 0, 0, 0, 0, 0, 0, 0 };
    for (auto it = begin(spellArray); it != end(spellArray); ++it) {
        *it = nullSpell;
    }
}

template<size_t N>
void clearAutoActionArray(std::array<SrAutoAction, N>& spellArray) {
    const SrAutoAction nullAction = { AUTOACT_NO_ACTION, 0xFF, 0xFF, 0, 0 };
    for (auto it = begin(spellArray); it != end(spellArray); ++it) {
        *it = nullAction;
    }
}

SISTERRAY_API void addAutoAction(u8 partyIndex, AutoActionType type, u8 commandIndex, u16 actionID, u8 activationChance, u8 counterCount);

/*Public API for getting and enabling stuff in your own enabling handlers*/
SISTERRAY_API void enableCommand(u8 partyIndex, u8 enabledIndex, u8 commandIndex);
SISTERRAY_API void voidCommand(u8 partyIndex, u8 enabledIndex);
SISTERRAY_API u8 getEnabledSlotIndex(u8 partyIndex, u8 commandIndex);
SISTERRAY_API void  insertEnabledCommand(u8 partyIndex, u8 commandIndex);
SISTERRAY_API EnabledCommand* getCommandSlot(u8 partyIndex, u8 commandIndex);

SISTERRAY_API EnabledSpell* getSpellSlot(u8 partyIndex, u8 commandIndex, u16 actionIndex);
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u8 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableMagic(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u8 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableSummon(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u8 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableESkill(u8 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);
#endif // !PARTY_ACTIONS_H
