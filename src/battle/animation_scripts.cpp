#include "animation_scripts.h"
#include "../impl.h"

NewActorAnimScripts::NewActorAnimScripts() {
    transformScript = (u32)&(transformationScript);
}

/*Replace the current animation script function for command index 0x23 with a new one*/
SISTERRAY_API void animationScriptTrampoline(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2) {
    u32 ptr_to_new_animation;
    if ((gBigAnimBlock[actor_id].commandID == CMD_LIMIT) && (gBigAnimBlock[actor_id].commandAnimID == 0x3D)) {
        srLogWrite("Redirecting animation script execution for actor %i", actor_id);
        srLogWrite("Writing to address %p, base %p", &gBigAnimBlock[actor_id].commandAnimID, &gBigAnimBlock[0].commandAnimID);
        ptr_to_new_animation = (u32)&(actorAnimArray);
        srLogWrite("actor animation address:%p, passed address %p", &actorAnimArray, ptr_to_new_animation);

    }
    else {
        ptr_to_new_animation = (u32)ptr_to_anim_scripts;
    }

    oldRunAnimationScript(actor_id, (u32)ptr_to_new_animation, unk1, unk1);
}

void srExecuteAnimationScript(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2) {

}
