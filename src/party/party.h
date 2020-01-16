#ifndef PARTY_H
#define PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "party_member.h"
#include "battle_stats.h"
#include "../battle/battle_actors.h"
#include "party_actions.h"

/*Holds extensible Enabled command arrays for active party members
  Will extend to hold data for all party members later to facilitate character swapping*/
class SrPartyMembers {
public:
    SrPartyMembers();
    PartyMemberState getActivePartyMember(u8 actorIdx);
    PartyMemberState getSrPartyMember(u8 actorIdx);
    void addAutoAction(u32 partyIndex, const SrAutoAction& action);
    void handleMateriaActorUpdates(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias);
    void clearActions(u32 partyIndex);
    void initPartyBattleFields(u8 partyIdx, const ActorBattleState& actorState);
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

void srRecalculateDerivedStats(u8 partyIndex);
void srUpdatePartyMember(u8 partyIndex);
void clearCommandArray(u8 partyIndex);
void enableDefaultCommands(u8 partyIndex, bool magicEnabled, bool summonEnabled);
void applyLinkedMateriaModifiers(u8 partyIndex, const std::vector<MateriaInventoryEntry>& equippedMaterias, SrGearType gearType);
void dispatchSupportHandlers(u8 partyIndex, const MateriaInventoryEntry& supportMateria, const MateriaInventoryEntry& pairedMateria, SrGearType gearType);

u8* getMateriaSlots(u8 partyIndex, SrGearType gearType);
bool slotsAreLinked(u8 leftSlot, u8 rightSlot);
void updateCommandsActive(u8 partyIndex, i32 commandType);
bool updateMagicCommand(u8 partyIndex, u32 actorStatusMask);
bool updateSummonCommand(u8 partyIndex, u32 actorStatusMask);
bool updateESkillCommand(u8 partyIndex, u32 actorStatusMask);
void updateCommands(i32 partyIndex, i16 statusMask);
u8 getCommandRows(u8 partyIndex);



#endif
