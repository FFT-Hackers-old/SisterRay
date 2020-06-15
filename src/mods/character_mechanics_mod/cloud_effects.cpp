#include "cloud_effects.h"
#include "../../impl.h"
#include "../../battle/battle_engine_api.h"
#include "../../gamedata/element_names.h"
#include "../../gamedata/damage_callback_utils.h"


static const auto gameLoadCrossSlash = (PFNSR_VOIDSUB)0x5AE4C0;
void setupSrCrossSlash(u16 actionTargetMask, u8 casterIdx) {
    gameLoadCrossSlash();
    srDelayPushTargetedEffect100(actionTargetMask, casterIdx, 0, (SRPFNSPELLEFFECTCALLBACK)srNewCrossSlashSetup);
}

void srNewCrossSlashSetup(u16 targetIdx, u16 casterIdx) {
    srPushEffect100(srCrossSlashMain);
    auto crossCtx = (CrossMainEffectCtx*)getEffect100QueueTop();
    crossCtx->targetIdx = targetIdx;
    setEffectDisplayPoint(targetIdx, getBattleModelState(targetIdx)->bData0x12[0], &crossCtx->displayPoint);
    crossCtx->scale = getBattleModelState(targetIdx)->height;
}

static const auto G_CROSS_SLASH_TEXTURES = (ActionEffectTextures*)0xBDA464;
void srCrossSlashMain() {
    auto crossMainCtx = (CrossMainEffectCtx*)getEffect100QueueTop();
    if (*BATTLE_PAUSED_GLOBAL)
        return;

    if (!crossMainCtx->frameCount) {
        srLoadActionSoundEffect(18, 0);
        srLoadActionSoundEffect(18, 0);
        srLoadActionSoundEffect(27, 0);
        srLoadActionSoundEffect(33, 0);
    }
    if (crossMainCtx->frameCount == 5) {
        srPushEffect100(slashEffectHandler);
        auto firstCtx = (SlashCtx*)getEffect100QueueTop();
        firstCtx->effectSPT = srGetEffectSPT(0, G_CROSS_SLASH_TEXTURES);
        firstCtx->effectDrawable = srGetEffectDrawable(0, G_CROSS_SLASH_TEXTURES);
        firstCtx->wasDispatchedFrame = crossMainCtx->frameCount;
        setPoint(&firstCtx->displayPoint, crossMainCtx->displayPoint.x, crossMainCtx->displayPoint.y, crossMainCtx->displayPoint.z);
        firstCtx->scale = crossMainCtx->scale;
        srDispatchTargetReaction(crossMainCtx->targetIdx);
    }
    if (crossMainCtx->frameCount == 9) {
        srPlayActionSoundEffect(32, srComputeMixPoint(&crossMainCtx->displayPoint), 18);
    }
    if (crossMainCtx->frameCount == 15) {
        srPushEffect100(slashEffectHandler);
        auto secondCtx = (SlashCtx*)getEffect100QueueTop();
        secondCtx->effectSPT = srGetEffectSPT(1, G_CROSS_SLASH_TEXTURES);
        secondCtx->effectDrawable = srGetEffectDrawable(1, G_CROSS_SLASH_TEXTURES);
        secondCtx->wasDispatchedFrame = crossMainCtx->frameCount;
        setPoint(&secondCtx->displayPoint, crossMainCtx->displayPoint.x, crossMainCtx->displayPoint.y, crossMainCtx->displayPoint.z);
        secondCtx->scale = crossMainCtx->scale;
        srDispatchTargetReaction(crossMainCtx->targetIdx);
    }
    if (crossMainCtx->frameCount == 19) {
        srPlayActionSoundEffect(32, srComputeMixPoint(&crossMainCtx->displayPoint), 18);
    }
    if (crossMainCtx->frameCount == 26) {
        srPushEffect100(slashEffectHandler);
        auto thirdCtx = (SlashCtx*)getEffect100QueueTop();
        thirdCtx->effectSPT = srGetEffectSPT(2, G_CROSS_SLASH_TEXTURES);
        thirdCtx->effectDrawable = srGetEffectDrawable(2, G_CROSS_SLASH_TEXTURES);
        thirdCtx->wasDispatchedFrame = crossMainCtx->frameCount;
        setPoint(&thirdCtx->displayPoint, crossMainCtx->displayPoint.x, crossMainCtx->displayPoint.y, crossMainCtx->displayPoint.z);
        thirdCtx->scale = crossMainCtx->scale;
        srDispatchTargetReaction(crossMainCtx->targetIdx);
    }
    if (crossMainCtx->frameCount == 30) {
        srPlayActionSoundEffect(32, srComputeMixPoint(&crossMainCtx->displayPoint), 27);
    }
    if (crossMainCtx->frameCount == 37) {
        srPlayActionSoundEffect(32, srComputeMixPoint(&crossMainCtx->displayPoint), 33);
        crossMainCtx->handlerControl = -1;
        srFlushActionSoundEffect(18, 15);
        srFlushActionSoundEffect(18, 15);
        srFlushActionSoundEffect(27, 15);
        srFlushActionSoundEffect(33, 15);
    }
    ++crossMainCtx->frameCount;

}


static const auto swishEffectHandler = (PFNSR_VOIDSUB)0x5AEAD6;
void slashEffectHandler() {
    u32* dword_BDA460 = (u32*)0xBDA460;
    u32* dword_BE1128 = (u32*)0xBE1128;
    TexSPTAnimationCtx texAnimCtx;
    auto slashEffect = (SlashCtx*)getEffect100QueueTop();
    texAnimCtx.effectSPT = slashEffect->effectSPT;
    texAnimCtx.effectDrawable = slashEffect->effectDrawable;
    texAnimCtx.field_C = 0;
    if (slashEffect->frameCount >= 4) {
        auto globalFrame = slashEffect->wasDispatchedFrame + slashEffect->frameCount;
        if (globalFrame >= 37) {
            if (globalFrame >= 45) {
                slashEffect->wasDispatchedFrame = -1;
                return;
            }
            if (!*BATTLE_PAUSED_GLOBAL) {
                srPushEffect100(swishEffectHandler);
                auto swishEffect = (SlashCtx*)getEffect100QueueTop();
                swishEffect->frameCount = 3 * globalFrame / 4;
                setPoint(&swishEffect->displayPoint, slashEffect->displayPoint.x, slashEffect->displayPoint.y, slashEffect->displayPoint.z);
                swishEffect->effectSPT = slashEffect->effectSPT;
                swishEffect->effectDrawable = slashEffect->effectDrawable;
                swishEffect->wasDispatchedFrame = globalFrame;
                swishEffect->scale = slashEffect->scale;
            }
        }
        else {
            texAnimCtx.color[2] = 0x80;
            texAnimCtx.color[1] = 0x80;
            texAnimCtx.color[0] = 0x80;
            texAnimCtx.color[3] = 46;
            //*(_DWORD*)a2 = 4096;
            srScaleTexture(&slashEffect->displayPoint, 4096, slashEffect->scale);
            *dword_BDA460 = srAnimateSPTTexture(&texAnimCtx, (u32*)(*dword_BE1128) + 0x70, 12, *dword_BDA460);
        }
    }
    else {
        texAnimCtx.color[2] = (slashEffect->frameCount << 7) / 4;
        texAnimCtx.color[1] = texAnimCtx.color[2];
        texAnimCtx.color[0] = texAnimCtx.color[2];
        texAnimCtx.color[3] = 46;
        auto v0 = (-12288 * slashEffect->frameCount >> 31) & 3;
        auto v1 = -12288 * slashEffect->frameCount;
        //*(_DWORD*)a2 = -12288 * slashEffect->frameCount / 4 + 0x4000;
        srScaleTexture(&slashEffect->displayPoint, ((v0 + v1) >> 2) + 0x4000, slashEffect->scale);
        *dword_BDA460 = srAnimateSPTTexture(&texAnimCtx, (u32*)(*dword_BE1128) + 0x70, 12, *dword_BDA460);
    }
    if (!*BATTLE_PAUSED_GLOBAL)
        ++slashEffect->frameCount;
}
