#include "action_effect_textures.h"

TextureSPT* srGetEffectSPT(u8 texEffectIdx, ActionEffectTextures* effectArray) {
    TextureSPT* ret = nullptr;
    if (effectArray && effectArray->effectArray) {
        ret = effectArray->effectArray[texEffectIdx].texSPT;
        effectArray->field_18 = effectArray->field_1C;
    }
    return ret;
}

GameDrawable* srGetEffectDrawable(u8 texEffectIdx, ActionEffectTextures* effectArray) {
    GameDrawable* ret = nullptr;
    if (effectArray && effectArray->effectArray) {
        ret = effectArray->effectArray[texEffectIdx].effectDrawable;
        effectArray->field_18 = effectArray->field_1C;
    }
    return ret;
}


typedef void(*PFNSR_GAMESCALETEX)(R3PointWord*, i32, i16);
static const auto gameScaleTexture = ((PFNSR_GAMESCALETEX)0x5D2B44);
void srScaleTexture(R3PointWord* point, i32 identityScale, i16 translationScale) {
    gameScaleTexture(point, identityScale, translationScale);
}


typedef i32(*PFNSR_GAMEANIMATESPT)(TexSPTAnimationCtx*, u32*, u32, u32);
static const auto gameAnimateSPTTexture = ((PFNSR_GAMEANIMATESPT)0x5D1FFB);
i32 srAnimateSPTTexture(TexSPTAnimationCtx* texAnimCtx, u32* a2, u32 a3,  u32 retVal) {
    return gameAnimateSPTTexture(texAnimCtx, a2, a3, retVal);
}
