#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "polygons.h"
#include "math.h"
#include "texture.h"
#include <windef.h>
#include <ddstream.h>

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
    void *d3dColorPal;
    void *rgbaPalette;
    void *paletteEntry;
    u32 ddPalette;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
} Palette;

typedef struct {
    u8 color_op;
    u8 field_1;
    u8 scroll_uv;
    u8 scroll_v;
    u8 change_palette;
    u8 setrenderstate;
    u8 add_offsets;
    u8 field_7;
    u32 color;
    u32 field_C;
    u32 field_10;
    u32 x_offset;
    u32 y_offset;
    u32 z_offset;
    u32 z_offset2;
    u32 u_offset;
    u32 v_offset;
    u32 palette_index;
    AuxillaryGFX* auxillaryData;
    u8 field_34[16];
} PaletteAuxillary;

typedef struct {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    TextureSet *textureSet;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 shadeMode;
    u32 lightstateAmbient;
    u32 field_2C;
    void *lightstate_material_pointer;
    u32 srcBlend;
    u32 dstBlend;
    u32 field_3C;
    u32 alpharef;
    u32 blend_mode;
    u32 zSort;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 vertexAlpha;
    u32 field_60;
} AuxillaryGFX;

typedef struct {
    u32 version;
    u32 field_4;
    u32 vertexType;
    u32 numVertices;
    u32 numNormals;
    u32 field_14;
    u32 numTexCoords;
    u32 numVertexColors;
    u32 numEdges;
    u32 numPolygons;
    u32 field_28;
    u32 field_2C;
    u32 numHundreds;
    u32 numGroups;
    u32 numBoundingBoxes;
    u32 has_normindextable;
    R3Point* vertices;
    R3Point* normals;
    R3Point* field_48;
    TextureCoords* texCoords;
    u32 *vertexColorData;
    u32 *polyColorData;
    PolygonEdge* edges;
    Polygon* polygons;
    char *pc_name;
    void *field_64;
    AuxillaryGFX* auxillaries;
    PGroup* groups;
    BoundingBox* boundingboxes;
    u32 *normindextabledata;
    u32 field_78;
    struct polygon_lists *lists;
} PFilePolygonData;

typedef struct {
    float blue;
    float green;
    float red;
    float alpha;
} ColorBGRA;

typedef struct {
    u32 field_0;
    u32 size;
    u32 field_8;
    u32 field_C;
    Matrix* matrixArray;
    Matrix* matrixWorld;
    Matrix* matrixView;
    Matrix* matrixProjection;
    void *d3dMatrix_World;
    void *d3dMatrix_View;
    void *d3dMatrix_Projection;
} MatrixSet;

typedef struct {
    u32 flags;
    u32 field_4;
    struct struc_106 *struc_106_1;
    struct struc_106 *struc_106_2;
    struct struc_106 *struc_106_3;
    ColorBGRA d3dcol4;
    ColorBGRA normd3dcol4;
    u32 color4;
    Matrix field_38;
    Matrix field_78;
    Matrix field_B8;
    Matrix field_F8;
    u32 field_138;
    Matrix field_13C;
    u32 field_17C;
    u32 field_180;
    u32 field_184;
    u32 field_188;
    u32 field_18C;
    u32 field_190;
    u32 field_194;
    u32 field_198;
    Matrix *matrix_pointer;
    u32 field_1A0;
    u32 field_1A4[256];
    u32 field_5A4;
    u32 color;
} LightGFX;

typedef struct {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;
    u32 numgroups;
    struct struc_49 field_14;
    u32 field_2C;
    PFilePolygonData* pFilePolygons;
    AuxillaryGFX* auxillaries;
    u32 hasPerGroupAuxillaies;
    AuxillaryGFX** groupAuxillaries;
    MatrixSet *matrixSet;
    LightGFX *light;
    u32 field_48;
    void *executeBuffers;			// IDirect3DExecuteBuffer **
    struct indexed_primitive **indexed_primitives;
    u32 field_54;
    u32 field_58;
    PolygonGroup *polygonGroups;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    u32 field_8C;
    u32 field_90;
    u32 hasPaletteAuxillary;
    u32 field_98;
    PaletteAuxillary* paletteAuxillary;
    u32 field_A0;
    u32 field_A4;
    u32 field_A8;
    u32 field_AC;
} PolygonSet;

typedef struct {
    u32 field_0;
    u32 field_4;
    u32 count;
    u32 vertexCount;
    u32 field_10;
    struct nvertex* vertices;
    u32 indexCount;
    u32 field_1C;
    u16* indices;
    u32 field_24;
    u8* palettes;
    u32 field_2C;
    GraphicsObject* graphicsObject;
} IndexedVertices;

typedef struct{
    u32 polytype;
    u32 field_4;
    u32 field_8;
    AuxillaryGFX* auxillaries;
    MatrixSet* matrixSet;
    PolygonSet* polygonSet;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    float u_offset;
    float v_offset;
    void *dx_sfx_2C;
    void *graphics_instance;
    u32 field_34;
    u32 verticesPerShape;
    u32 indicesPerShape;
    u32 vertexOffset;
    u32 indexOffset;
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
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    IndexedVertices *indexedVertices;
    SRGFXDRIVER_SETPOLYRENDER *func_90;
    SRGFXDRIVER_DRAWVERTICES *func_94;
    u32 useMatrixPointer;
    Matrix* matrixPtr;
    Matrix matrix;
} GraphicsObject;

#endif // !GRAPHICS_H
