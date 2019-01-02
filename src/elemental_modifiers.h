#ifndef ELEMENTAL_HEADER_H
#define ELEMENTAL_HEADER_H

#include <SisterRay/SisterRay.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>

SISTERRAY_API void ApplyElementalModifiers();
SISTERRAY_API void HandleElementalInteractions();

SISTERRAY_API void InflictElementalStatus();
SISTERRAY_API void HandleElementalInteractions();
void applyBurnedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id);
void applySoakedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id);
void applyFloatInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id);
u16 getAllowableArcTargets(u32 source_target);
void handleInflictBurn(u32 target_id);
void handleInflictChilled(u32 target_id);
void handleInflictSoaked(u32 target_id);
void handleInflictBleed(u32 target_id);
void handleLightingArc(u32 target_id);
void handleInflictFloat(u32 target_id);

#endif ELEMENTAL_HEADER_H