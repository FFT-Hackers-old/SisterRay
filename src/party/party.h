#ifndef PARTY_H
#define PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "stat_boosts.h"
#include <array>

#define MAGIC_COUNT 64
#define SUMMON_COUNT 16
#define ESKILL_COUNT 24
#define AUTO_ACTION_COUNT 10

/*These are relocated from the games base locations to make them extensible*/
typedef struct {
    std::array<EnabledSpell, MAGIC_COUNT> actorMagics;
    std::array<EnabledSpell, SUMMON_COUNT> actorSummons;
    std::array<EnabledSpell, ESKILL_COUNT> actorEnemySkills;
    std::array<SrAutoAction, AUTO_ACTION_COUNT> actorAutoActions;
} SrPartyData;

/*Holds extensible Enabled command arrays for active party members
  Will extend to hold data for all party members later to facilitate character swapping*/
class SrPartyDataRegistry : public SrNamedResourceRegistry<SrPartyData, std::string> {
public:
    SrPartyDataRegistry();
    void addAutoAction(u32 partyIndex, const SrAutoAction& action);
    void handleMateriaActorUpdates(u32 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, ActorStatBoosts& boosts);
    void clearActions(u32 partyIndex);
};

template<size_t N>
void clearActionArray(std::array<EnabledSpell, N>& spellArray) {
    const EnabledSpell nullSpell = { 0xFF, 0, 0, 0, 0, 0, 0, 0 };
    for (auto it = begin(spellArray); it != end(spellArray); ++it) {
        *it = nullSpell;
    }
}

template<size_t N>
void clearAutoActionArray(std::array<SrAutoAction, N>& spellArray) {
    const SrAutoAction nullAction = {AUTOACT_NO_ACTION, 0xFF, 0xFF, 0, 0};
    for (auto it = begin(spellArray); it != end(spellArray); ++it) {
        *it = nullAction;
    }
}

void srRecalculateDerivedStats(u32 partyIndex);
const std::string getPartyKey(u8 partyIndex);
void srUpdatePartyMember(u32 partyIndex);
void clearCommandArray(u8 partyIndex);
void enableDefaultCommands(u8 partyIndex, bool magicEnabled, bool summonEnabled);
void applyLinkedMateriaModifiers(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType, ActorStatBoosts& boosts);
void dispatchSupportHandlers(u8 partyIndex, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGearType gearType, ActorStatBoosts& boosts);

SISTERRAY_API void addAutoAction(u8 partyIndex, AutoActionType type, u8 commandIndex, u16 actionID, u8 activationChance, u8 counterCount);

/*Public API for getting and enabling stuff in your own enabling handlers*/
SISTERRAY_API void enableCommand(u8 partyIndex, u8 enabledIndex, u8 commandIndex);
SISTERRAY_API void voidCommand(u8 partyIndex, u8 enabledIndex);
SISTERRAY_API u8 getEnabledSlotIndex(u8 partyIndex, u8 commandIndex);
SISTERRAY_API void  insertEnabledCommand(u8 partyIndex, u8 commandIndex);
SISTERRAY_API EnabledCommandStruct* getCommandSlot(u8 partyIndex, u8 commandIndex);

SISTERRAY_API EnabledSpell* getSpellSlot(u8 partyIndex, u8 commandIndex, u16 actionIndex);
SISTERRAY_API EnabledSpell* getEnabledMagicSlot(u32 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableMagic(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledSummonSlot(u32 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableSummon(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);
SISTERRAY_API EnabledSpell* getEnabledESkillSlot(u32 partyIndex, u32 enabledSlotIndex);
SISTERRAY_API void enableESkill(u32 partyIndex, u32 enabledIndex, u32 commandlRelativeIndex);


#endif
