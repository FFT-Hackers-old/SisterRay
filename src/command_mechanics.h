#include <SisterRay/SisterRay.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>

SISTERRAY_API void CommandMainRewrite(u8* cmd);
SISTERRAY_API int TriggerCloudSpecial();
SISTERRAY_API void ModifyPoisonTest();
SISTERRAY_API void AnimationScriptRewrite(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2);
SISTERRAY_API void PoisonCallbackRewrite(u32 actor_id);
SISTERRAY_API void DecrementCountersRewrite();
//SISTERRAY_API void AnimationEffectRewrite(u8 actor_id);

unsigned char newAnimScript[] = {0xE8, 0xFC, 0x00, 0xED, 0xE6, 0xEA, 0x00, 0x00, 0xEC, 0x00, 0x00, 0xFA, 0xE5, 0xEE};
unsigned char newAnimScript2[] = {0xE8, 0xEC, 0x9E};
unsigned char newAnimScriptEnemies[] = { 0xE8, 0xEC, 0x9E };

/*We require an array of */
typedef struct {
	u32 pad1;
	u32 pad2;
	u32 pad3;
	u32 ptrToEffectScript1;
	u32 ptrToEffectScript2;
} newActorAnimPtr;


/*create a new array for holding ptrs to new animations*/
newActorAnimPtr actorAnimArray[10] =  {
	{0x00, 0x00, 0x00, (u32)&newAnimScript2, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScript2, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScript2, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScript2, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript},
	{0x00, 0x00, 0x00, (u32)&newAnimScriptEnemies, (u32)&newAnimScript}
};