#ifndef PARTY_EVENTS_H
#define PARTY_EVENTS_H

#include "event.h"
#include "../party/stat_boosts.h"

/*For the public API events should be C-Standard compliant*/
typedef struct {
    const u32 partyIndex;
    const MateriaInventoryEntry materia;
    const MateriaData materiaData;
    const u8 materiaLevel;
    ActorStatBoosts* statBoosts;
} EnableAbilitiesEvent;

typedef struct {
    const u32 partyIndex;
    const MateriaInventoryEntry supportMateria;
    const u8 supportMateriaLevel;
    const MateriaData SupportMateriaData;
    const MateriaInventoryEntry pairedMateria;
    const u8 pairedMateriaLevel;
    const MateriaData pairedData;
    ActorStatBoosts* statBoosts;
    SrGearType gearType;
} ApplySupportEvent;

#endif
