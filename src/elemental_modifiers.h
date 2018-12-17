#include <SisterRay/SisterRay.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>

SISTERRAY_API void ApplyElementalModifers();
SISTERRAY_API void HandleElementalInteractions();

SISTERRAY_API void InflictElementalStatus();
SISTERRAY_API void HandleElementalInteractions();
void applyBurnedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id);
void applySoakedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id);

/*Struct for holding actor data for new status effects*/
typedef struct {
    u16 bleedDuration;
    u16 bleedIntensity;
    u16 bleedTickRate;
    u16 burnDuration;
    u16 burnIntensity;
    u16 burnTickRate;
    u16 chilledDuration;
    u16 feezeDuration;
    u16 soakedDuration;
} newStatusActorBlock;

static u16 defaultBleedDuration = 0x50;
static u16 defaultBleedTick = 0x14;
static u16 defaultBleedIntensity = 0x01;
static u16 defaultBurnDuration = 0x80;
static u16 defaultBurnTick = 0x14;
static u16 defaultBurnIntensity = 0x01;
static u16 defaultSoakedDuration = 0x60;

/*create a new array for holding counters associated with new status effects*/
newStatusActorBlock statusConstantArray[10];
