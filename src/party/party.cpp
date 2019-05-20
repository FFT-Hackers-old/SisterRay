#include "party.h"

SrPartyDataRegistry::SrPartyDataRegistry() {
    add_element("1_PARTY", SrPartyData());
    add_element("2_PARTY", SrPartyData());
    add_element("3_PARTY", SrPartyData());
}

void srUpdatePartyMember(u32 partyIndex) {
    recalculateBaseStats(partyIndex);
    recalculateDerivedStats(partyIndex); //This function will be replaced with an sr version to correctly init the new command fields
    updateMiscPartyStats();
}
