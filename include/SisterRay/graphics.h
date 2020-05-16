#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "polygons.h"
#include "math.h"
#include "texture.h"
#include "data_structures.h"

typedef struct _GraphicsObject GraphicsObject;
typedef struct _PolygonSet PolygonSet;

typedef struct{
    u32 field_0;
    u32 zSort;
    u32 vertexAlpha;
    u32 srcBlendMode;
    u32 srcBlendCaps;
    u32 dstBlendMode;
    u32 dstBlendCaps;
    u32 field_1C;
    u32 field_20;
} BlendMode;

#pragma pack(push, 1)
typedef struct {
    u32 frameCounter;
    GameHeap* heap;
} GraphicsInstance;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 actionFlags;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    TextureSet *textureSet;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 shadeMode;
    u32 lightstateAmbient;
    u32 drainedHP;
    void *lightstateMaterialPointer;
    u32 srcBlend;
    u32 dstBlend;
    u32 field_3C;
    u32 alpharef;
    u32 blendMode;
    u32 zSort;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 vertexAlpha;
    u32 field_60;
} AuxillaryGFX;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u8 colorOp;
    u8 field_1;
    u8 scrollUV;
    u8 scrollV;
    u8 changePalette;
    u8 setRenderState;
    u8 addOffsets;
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
    u32 paletteIndex;
    AuxillaryGFX* auxillaryData;
    u8 innateStatusMask[16];
} PaletteAuxillary;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 version;
    u32 actionFlags;
    u32 vertexType;
    u32 numVertices;
    u32 numNormals;
    u32 field_14;
    u32 numTexCoords;
    u32 numVertexColors;
    u32 numEdges;
    u32 numPolygons;
    u32 field_28;
    u32 drainedHP;
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
    GamePolygonEdge* edges;
    GamePolygon* polygons;
    char *pc_name;
    void *field_64;
    AuxillaryGFX* auxillaries;
    PGroup* groups;
    BoundingBox* boundingboxes;
    u32 *normindextabledata;
    u32 field_78;
    GameList** polygonLists; //Reverse engineer later
} PFilePartHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    float blue;
    float green;
    float red;
    float alpha;
} ColorBGRA;
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 colorRGBA;
    R3Point point;
    ColorBGRA colorBGRA;
} LightPoint;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 flags;
    u32 actionFlags;
    LightPoint* lightPoint8;
    LightPoint* lightPointC;
    LightPoint* lightPoint10;
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
    Matrix *matrixPointer;
    u32 field_1A0;
    u32 field_1A4[256];
    u32 field_5A4;
    u32 color;
} LightGFX;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    R3Point _;
    union
    {
        struct
        {
            float w;
            union
            {
                u32 color;
                struct
                {
                    u8 b;
                    u8 g;
                    u8 r;
                    u8 a;
                };
            };
            u32 specular;
        } color;

        R3Point normal;
    };
    float u;
    float v;
} nVertex;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 vertexSize;
    u32 primitiveType;
    u32 vertexType;
    nVertex* vertices;
    u32 vertexCount;
    u16* indices;
    u32 indexCount;
    u32 flags;
    u32 field_24;
} IndexedPrimitive;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    GraphicsObject* graphicsObject;
    u32 polygontype;
    u32 field_8;
    R3Point vertices[4];
    TextureCoords textureCoords[4];
    u32 colors[4];
    float w[4];
    nVertex *nvertexPointer;
    u32 paletteIndex;
} DrawableState;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    void* nextDrawable; //type is DrawableObjectCahin, cast if using
    u32 actionFlags;
    DrawableState* drawableState;
    Matrix matrix;
    PaletteAuxillary paletteAuxillary;
} DrawableObjectChain;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 actionFlags;
    u32 field_8;
    u32 frameCounter;
    DrawableObjectChain* graphicsObjectChain;
    GraphicsInstance *graphicsInstance;
} GraphicsChain;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    void* next; //type is PolyonSetChain*, cast if using
    u32 currentGroup;
    PolygonSet* polygonSet;
    AuxillaryGFX* auxillary;
    u32 useMatrix;
    Matrix matrix;
    u32 use_matrix_pointer;
    Matrix* matrix_pointer;
    PaletteAuxillary paletteAuxillary;
} PolygonSetChain;
#pragma pack(pop)

#pragma pack(push, 1)
struct _PolygonSet {
    u32 field_0;
    u32 actionFlags;
    u32 field_8;
    u32 field_C;
    u32 numgroups;
    GraphicsChain field_14;
    u32 drainedHP;
    PFilePartHeader* pFilePolygons;
    AuxillaryGFX* auxillaries;
    u32 hasPerGroupAuxillaies;
    AuxillaryGFX** groupAuxillaries;
    MatrixSet *matrixSet;
    LightGFX *light;
    u32 field_48;
    void *executeBuffers;			// IDirect3DExecuteBuffer **
    IndexedPrimitive** indexedPrimitives;
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
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u32 field_0;
    u32 actionFlags;
    u32 count;
    u32 vertexCount;
    u32 field_10;
    nVertex* vertices;
    u32 indexCount;
    u32 field_1C;
    u16* indices;
    u32 field_24;
    u8* palettes;
    u32 drainedHP;
    GraphicsObject* graphicsObject;
} IndexedVertices;
#pragma pack(pop)

#pragma pack(push, 1)
struct _GraphicsObject {
    u32 polytype;
    u32 actionFlags;
    u32 field_8;
    AuxillaryGFX* auxillaries;
    MatrixSet* matrixSet;
    PolygonSet* polygonSet;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    float uOffset;
    float vOffset;
    void *dx_sfx_2C;
    void *graphicsInstance;
    u32 innateStatusMask;
    u32 verticesPerShape;
    u32 indicesPerShape;
    u32 vertexOffset;
    u32 indexOffset;
    Matrix rotationMatrix;
    IndexedVertices *indexedVertices;
    void* renderPolygonsCallback; //function type is SRGFXDRIVER_SETPOLYRENDER
    void* drawVerticesCallback;  //function type is SRGFXDRIVER_DRAWVERTICES
    u32 useMatrixPointer;
    Matrix* matrixPtr;
    Matrix matrix;
};
#pragma pack(pop)

#endif // !GRAPHICS_H
