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
u16 getAllowableArcTargets(u32 source_target);
u16 getRandomBit(u16 bit_vector);
int isBitSet(u16 vector, int bit);
