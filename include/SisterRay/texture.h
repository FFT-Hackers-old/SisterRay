#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"

typedef struct {
    float u;
    float v;
} TextureCoords;

#pragma pack(push, 1)
typedef struct {
    u32 width;
    u32 height;
    u32 bytesPerRow;
    void *field_C;
    u32 usePalette;
    u32 bitsPerIndex;
    u32 indexedTo8bit;
    u32 paletteSize;
    u32 palettes;
    u32 *paletteData;
    u32 bitsperpixel;
    u32 bytesperpixel;
    u32 redBits;
    u32 greenBits;
    u32 blueBits;
    u32 alphaBits;
    u32 redMask;
    u32 greenMask;
    u32 blueMask;
    u32 alphaMask;
    u32 redShift;
    u32 greenShift;
    u32 blueShift;
    u32 alphaShift;
    u32 redTrueColor_bitDiff;
    u32 greenTrueColor_bitDiff;
    u32 blueTrueColor_bitDiff;
    u32 alphaTrueColor_bitDiff;
    u32 redMax;
    u32 greenMax;
    u32 blueMax;
    u32 alphaMax;
} TextureFormat;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 version;
    u32 field_4;
    u32 colorKey;
    u32 field_C;
    u32 field_10;
    u32 minBitsPerColor;
    u32 maxBitsPerColor;
    u32 minAlphaBits;
    u32 maxAlphaBits;
    u32 minBitsPerPixel;
    u32 maxBitsPerPixel;
    u32 field_2C;
    u32 paletteCount;					
    u32 paletteEntries;
    u32 bitsPerPixel;
    TextureFormat textureFormat;
    u32 usePaletteColorKey;
    u8 *paletteColorKey;
    u32 referenceAlpha;
    u32 blendMode;
    u32 field_CC;
    u32 paletteIndex;
    u8* imageData;
    u8* oldPaletteData;
    u32 field_DC;
    u32 field_E0;
    u32 field_E4;
    u32 field_E8;
} TextureHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 paletteSize;
    u32 bitsPerPixel;
    u32 field_C;
    u32 paletteEntries;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    void* d3dColorPal;
    void* rgbaPalette;
    void* paletteEntry;
    u32 ddPalette;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
} Palette;
#pragma pack(pop)

typedef struct {
    void *ddsurface1;
    void *d3d2texture1;
    void *ddsurface2;
    void *d3d2texture2;
    u32 field_10;
    u32 field_14;
    u32 refCount;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 *textureHandle;
    TextureFormat *textureFormat;
    TextureHeader *texHeader;
    u32 paletteIndex;
    Palette* palette;
    u32 field_90;
    u32 field_94;
    u32 field_98;
    u32 field_9C;
} TextureSet;

typedef struct {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 vertexColor;
    TextureSet* textureSet;
} unkTextureStructure;

#endif
