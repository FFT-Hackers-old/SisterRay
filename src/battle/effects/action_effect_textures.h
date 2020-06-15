#ifndef ACTION_EFFECT_TEXTURES_H
#define ACTION_EFFECT_TEXTURES_H

#include<SisterRay/SisterRay.h>

struct PageSPT {
    i16 x;
    i16 y;
    i16 uScale;
    i16 vScale;
    u16 field_C;
    u16 somethingPalette;
    i16 w;
    i16 h;
};

struct TexPageList {
    u32 field_0;
    PageSPT* pageSPT;
};

/* Texture SPT is created when effects are loaded prior
to an action being executed. It is an in-memory representation of the
.s files associated with action effects -- and controls texture animation*/
struct TextureSPT {
    u8* sptHandleCopy;
    TexPageList* pages;
    u8* sptHandle;
    u32 texPageCount;
    u32 field_10[4];
    u32 field_20[4];
};


struct GameEffectTexture {
    u32 type;
    GameDrawable* effectDrawable;
    TextureSPT* texSPT;
};

struct ActionEffectTextures {
    u32 effectCount;
    char* lgpPath;
    u32 field_8;
    GameEffectTexture* effectArray;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
};

struct TexSPTAnimationCtx {
    TextureSPT* effectSPT;
    GameDrawable* effectDrawable;
    u8 color[4];
    u16 field_C;
    u16 field_E;
};

TextureSPT* srGetEffectSPT(u8 effectIdx, ActionEffectTextures* effectsLoc);
GameDrawable* srGetEffectDrawable(u8 effectIdx, ActionEffectTextures* effectsLoc);
void srScaleTexture(R3PointWord* point, i32 identityScale, i16 translationScale);
i32 srAnimateSPTTexture(TexSPTAnimationCtx* texAnimCtx, u32* a2, u32 a3, u32 retVal);

#endif
