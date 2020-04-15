#ifndef PARTY_H
#define PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "party_member.h"
#include "battle_stats.h"
#include "../battle/battle_actors.h"
#include "characters.h"
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
    SrCharacter& getActivePartyCharacter(u8 partyIdx);
    void addAutoAction(u32 characterIdx, const SrAutoAction& action);
    void handleMateriaActorUpdates(u8 characterIdx, const std::vector<MateriaInventoryEntry>& equippedMaterias);
    void clearActions(u32 characterIdx);
    void initPartyBattleFields(u8 partyIdx, const ActorBattleState& actorState);
    void initCharacterBattleFields(u8 characterID, const ActorBattleState& actorState);
    void recalculateCharacter(u8 characterID);
    void recalculatePartyMember(u8 partyIdx);
    void swapPartyMembers(u8 partyIdx, u8 newCharacterID);
protected:
    void setPartyMemberActive(u8 partyIdx, u8 newCharacterID);
    void battleActivatePartyMember(u8 partyIdx);
    void battleSavePartyMember(u8 partyIdx);
private:
    std::array<u8, 3> activeParty;
    std::array<SrPartyData, 10> partyMembers;
    std::array<PartyMember, 10> gamePartyMembers;
};

PartyMemberState getSrPartyMember(u8 partyIdx);
PartyMemberState getActivePartyMember(u8 partyIdx);
PartyMemberState getSrCharacter(u8 characterIdx);

void srRecalculateDerivedStats(u8 characterIdx);
void srUpdatePartyMember(u8 characterIdx);
void clearCommandArray(u8 characterIdx);
void enableDefaultCommands(u8 characterIdx, bool magicEnabled, bool summonEnabled);
void applyLinkedMateriaModifiers(u8 characterIdx, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType);
void dispatchSupportHandlers(u8 characterIdx, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGearType gearType);

u8* getMateriaSlots(u8 characterIdx, SrGearType gearType);
bool slotsAreLinked(u8 leftSlot, u8 rightSlot);
void updateCommandsActive(u8 characterIdx, i32 commandType);
bool updateMagicCommand(u8 characterIdx, u32 actorStatusMask);
bool updateSummonCommand(u8 characterIdx, u32 actorStatusMask);
bool updateESkillCommand(u8 characterIdx, u32 actorStatusMask);
void updateCommands(i32 characterIdx, i16 statusMask);
u8 getCommandRows(u8 characterIdx);

#endif
