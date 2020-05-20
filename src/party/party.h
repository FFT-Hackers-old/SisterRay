#ifndef PARTY_H
#define PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "party_member.h"
#include "characters.h"
#include "../battle/battle_actors.h"
#include "../gamedata/battle_stats.h"
#include "party_interface.h"
#include <array>

/*Holds extensible Enabled command arrays for active party members
  Will extend to hold data for all party members later to facilitate character swapping*/

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

void initParty();

class SrPartyMembers {
public:
    SrPartyMembers() {}
    SrPartyMembers(u8 characterCount);
    PartyMemberState getActivePartyMember(u8 actorIdx);
    PartyMemberState getSrPartyMember(u8 actorIdx);
    PartyMemberState getSrCharacter(u8 characterID);
    PartyMemberState getSrSummon(u8 summonIdx);
    SrCharacter& getActivePartyCharacter(u8 partyIdx);
    void addAutoAction(u32 characterIdx, const SrAutoAction& action);
    void handleMateriaActorUpdates(u8 characterIdx, const std::vector<MateriaInventoryEntry>& equippedMaterias);
    void clearActions(u32 characterIdx);
    void clearSummonActions(u8 summonIdx);
    void initPartyBattleFields(u8 partyIdx, const ActorBattleState& actorState);
    void initCharacterBattleFields(u8 characterID, const ActorBattleState& actorState);
    void initSummonBattleFields(u8 summonIdx, const ActorBattleState& actorState);
    void recalculateCharacter(u8 characterID);
    void recalculatePartyMember(u8 partyIdx);
    void swapPartyMembers(u8 partyIdx, u8 newCharacterID);
    void activateSummon(u8 summonIdx);
    void battleDeactivatePartyMember(u8 partyIdx);
    bool isSlotEnabled(u8 partyIdx);
    void deactivateSlot(u8 partyIdx);
    void activateSlot(u8 partyIdx);
    void setSummonCtx(u8 summonIdx, u32 cumulativeAP, u8 maxLevel);
    const PartySummonCtx& getSummonCtx(u8 summonIdx);
    void initializePlayableSummons();
protected:
    void setPartyMemberActive(u8 partyIdx, u8 newCharacterID);
    void battleActivatePartyMember(u8 partyIdx);
    void battleSavePartyMember(u8 partyIdx);
private:
    std::array<u8, 3> activeParty;
    std::array<bool, 3> slotEnabled;
    std::array<bool, 3> actorIsSummon;
    u8 activeSummonIdx;
    std::array<SrPartyData, 10> partyMembers;
    std::array<PartyMember, 10> gamePartyMembers;
    std::array<PartySummonCtx, 16> summonCtx;
    std::array<SrPartyData, 16> summonMembers;
    std::array<PartyMember, 16> summonGameMembers;
};

PartyMemberState getSrPartyMember(u8 partyIdx);
PartyMemberState getActivePartyMember(u8 partyIdx);
PartyMemberState getSrCharacter(u8 characterIdx);

void srRecalculateDerivedStats(u8 characterIdx);
void srUpdatePartyMember(u8 characterIdx);
void clearCommandArray(u8 characterIdx);
void voidCommand(EnabledCommand& command, u8 enabledIndex);
void clearSummonCommandArray(u8 summonIdx);
void enableDefaultCommands(u8 characterIdx, bool magicEnabled, bool summonEnabled);
void applyLinkedMateriaModifiers(u8 characterIdx, u8* slots, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGameGearType gearType);
void dispatchSupportHandlers(u8 characterIdx, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGameGearType gearType);

bool slotsAreLinked(u8 leftSlot, u8 rightSlot);
void updateCommands(u8 partyIdx, u32 statusMask);
void updateCommandsActive(u8 characterIdx, i32 commandType);
bool updateMagicCommand(u8 characterIdx, u32 actorStatusMask);
bool updateSummonCommand(u8 characterIdx, u32 actorStatusMask);
bool updateESkillCommand(u8 characterIdx, u32 actorStatusMask);
void updateCommands(i32 characterIdx, i16 statusMask);
u8 getCommandRows(u8 characterIdx);

#endif
