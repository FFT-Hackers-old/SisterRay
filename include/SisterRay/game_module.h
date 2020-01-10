#ifndef GAME_MODULE_H
#define GAME_MODULE_H

#include "types.h"
#include "graphics.h"
#include "data_structures.h"
#include <windows.h>
#include <ddstream.h>

typedef void(*GAMEMODULEHANDLER)(void);

#pragma pack(push, 1)
typedef struct {
    GAMEMODULEHANDLER initialize;
    GAMEMODULEHANDLER destroy;
    GAMEMODULEHANDLER onEnterMain;
    GAMEMODULEHANDLER onExitMain;
    GAMEMODULEHANDLER main;
    u32 field_14;
    u32 field_18;
} GameMainCallbacks;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct{
    u8 dummy[0xCC];
} D3DDEVICEDESC;
#pragma pack(pop)

typedef struct _GameContext GameContext;
typedef struct _GraphicsDriverCallbacks GraphicsDriverCallbacks;
typedef struct _GraphicsObjectExternals GraphicsObjectExternals;

#pragma pack(push, 1)
struct _GameContext {
    u32 unk0;
    u32 width;
    u32 heiht;
    u32 bitsPerPixel;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    double field_28;
    double clockRateDouble;
    u64 clockRateInteger;
    u64 field_40;
    double fps;
    u32 tsc;
    u32 field_54;
    HINSTANCE hinstance;
    HWND hwnd;
    u32 exitValue;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    void* directDrawCtx;
    void* iDirectDraw2;
    void* surface[3];
    DDSURFACEDESC surfaceDesc2[3];
    void* surface2;
    u32 field_1D0;
    void* clipper; //DirectDrawClipper
    u32 field_1D8;
    DDSURFACEDESC surfaceDescD3D;
    void *directDrawCtx2;
    u32 field_24C;
    DDSURFACEDESC surfaceDesc;
    GameList *d3ddev_list;
    void *d3dinterface;
    void *surface_d3ddev;			// IDirect3DDevice
    GameList *textureFormat_List;
    void *d3ddev_struct;
    void *d3dviewport;
    void *d3dmaterial;
    u32 field_2D8;
    void *d3d2interface;
    void *d3d2device;
    void *d3dviewport2;
    GameList *list_2E8;
    PolygonSet* polygonSet2EC;
    PolygonSet* polygonSet2F0;
    void* matrixStack1;
    void* matrixStack2;
    Matrix* cameraMatrix;
    GraphicsInstance* graphicsInstance;
    u32 field_304;
    u32 field_308;
    u32 field_30C;
    u32 field_310;
    u32 field_314;
    u32 field_318;
    u32 field_31C;
    u32 field_320;
    D3DDEVICEDESC d3d_halcaps;
    D3DDEVICEDESC d3d_helcaps;
    DDCAPS_DX5 halcaps;
    DDCAPS_DX5 helcaps;
    u32 field_794;
    u32 field_798;
    TextureFormat surfaceTexture;
    u32 in_scene;
    AuxillaryGFX* auxillaries[5];
    void *applog_debug1;
    u32 applog_debug2;
    void *dxdbg_file;
    u32 field_840;
    u32 field_844;
    u32 _res_x;
    u32 _res_y;
    u32 _res_w;
    u32 _res_h;
    u32 field_858;
    u32 field_85C;
    u32 field_860;
    u32 field_864;
    u32 field_868;
    u32 field_86C;
    u32 field_870;
    u32 field_874;
    u32 field_878;
    u32 field_87C;
    u32 field_880;
    u32 field_884;
    u32 field_888;
    u32 field_88C;
    Matrix matrix_890;
    Matrix matrix_8D0;
    void *dx_sfx_something;
    GameList* textureListPtr;
    void* stack_918;
    u32 field_91C;
    void *_3d2d_something;
    u32 field_924;
    u32 field_928;
    u32 field_92C;
    u32 field_930;
    GraphicsDriverCallbacks* graphicsDriverCallbacks;
    void *_3dobject_pool;
    u32 field_93C;
    AuxillaryGFX *currentAuxillary;
    unkTextureStructure* someTextureStructure;
    u32 field_948;
    u32 field_94C;
    u32 field_950;
    u32 resWidth;
    u32 resHeight;
    u32 colorDepth;
    u32 field_960;
    u32 isFullScreen;
    u32 useGraphicsGUI;
    u32 graphicsOption;
    u32 field_970;
    u32 showCursor;
    u32 colorkey;
    u32 field_97C;
    u32 field_980;
    u32 d3d2_flag;
    u32 field_988;
    u32 field_98C;
    u32 field_990;
    u32 field_994;
    u32 matrixStackSize;
    u32 FieldOfView;
    u32 zNear;
    u32 zFar;
    u32 leftPosition;
    u32 topPosition;
    u32 randomSeed;
    char* wndNamePtr;
    char* wndClassNamePtr;
    u32 useWndClass;
    WNDCLASSA wndclass;
    u32 useDWStyle;
    u32 DWStyle;
    GameMainCallbacks mainCallbacks9F0;
    GameMainCallbacks mainCallbacksA0C;
    void *wm_activate;
    u32 field_A2C;
    u32 field_A30;
    u32 field_A34;
    u32 field_A38;
    u32 field_A3C;
    u32 field_A40;
    u32 field_A44;
    u32 field_A48;
    u32 field_A4C;
    u32 field_A50;
    u32 field_A54;
    u32 field_A58;
    u32 field_A5C;
    u32 currentGFXDriver;
    u32 field_A64;
    u32 field_A68;
    u32 field_A6C;
    u32 field_A70;
    u32 field_A74;
    u32 field_A78;
    void* GFXDriverData;
    u32 field_A80;
    u32 field_A84;
    void* createGFXDriver;
    GraphicsObjectExternals* externals;
    u32 nvidiaFix;
    u32 tntFix;
    u32 noRivaFix;
    u32 field_A9C;
};
#pragma pack(pop)

typedef u32(*SRGFXDRIVER_CREATE)(GameContext*);
typedef void(*SRGFXDRIVER_DESTROY)(GameContext*);
typedef u32(*SRGFXDRIVER_LOCK)(u32);
typedef u32(*SRGFXDRIVER_UNLOCK)(u32);
typedef void(*SRGFXDRIVER_FLIP)(GameContext*);
typedef void(*SRGFXDRIVER_CLEAR)(u32, u32, u32, GameContext*);
typedef void(*SRGFXDRIVER_CLEARALL)(GameContext*);
typedef void(*SRGFXDRIVER_SETVIEWPORT)(u32, u32, u32, u32, GameContext*);
typedef void(*SRGFXDRIVER_SETBG)(ColorBGRA *, GameContext*);
typedef u32(*SRGFXDRIVER_INITPOLYGONSET)(PolygonSet*);
typedef u32(*SRGFXDRIVER_LOADGROUP)(u32, MatrixSet*, AuxillaryGFX*, PGroup*, PFilePartHeader*, PolygonSet*, GameContext*);
typedef void(*SRGFXDRIVER_SETMATRIX)(u32, Matrix*, MatrixSet*, GameContext*);
typedef void(*SRGFXDRIVER_UNLOADTEXTURE)(TextureSet*);
typedef TextureSet*(*SRGFXDRIVER_LOADTEXTURE)(TextureSet*, TextureHeader*, TextureFormat*);
typedef u32(*SRGFXDRIVER_PALETTECHANGED)(u32, u32, u32, Palette*, TextureSet*);
typedef u32(*SRGFXDRIVER_WRITEPALLETTE)(u32, u32, void*, u32, Palette*, TextureSet*);
typedef BlendMode*(*SRGFXDRIVER_BLENDMODE)(u32, GameContext*);
typedef void(*SRGFXDRIVER_LIGHTPOLYGONSET)(PolygonSet*, void*);
typedef void(*SRGFXDRIVER_FIELD64CB)(u32, u32, GameContext*);
typedef void(*SRGFXDRIVER_SETRENDERSTATE)(AuxillaryGFX*, GameContext*);
typedef void(*SRGFXDRIVER_FIELD74CB)(u32, GameContext*);
typedef void(*SRGFXDRIVER_FIELD78CB)(PolygonSet*, GameContext*);
typedef void(*SRGFXDRIVER_DEFERREDDRAW)(PolygonSetChain*, GameContext*);
typedef void(*SRGFXDRIVER_FIELD80CB)(GraphicsObject*, GameContext*);
typedef void(*SRGFXDRIVER_FIELD84CB)(u32, GameContext*);
typedef u32(*SRGFXDRIVER_BEGINSCENE)(u32, GameContext*);
typedef void(*SRGFXDRIVER_ENDSCENE)(GameContext*);
typedef void(*SRGFXDRIVER_FIELD90CB)(u32);
typedef void(*SRGFXDRIVER_SETPOLYRENDER)(PolygonSet*, IndexedVertices*, GameContext*);
typedef void(*SRGFXDRIVER_DRAWVERTICES)(PolygonSet*, IndexedVertices*, GameContext*);
typedef void(*SRGFXDRIVER_FIELDECCB)(GameContext*);

#pragma pack(push, 1)
struct _GraphicsDriverCallbacks {
    SRGFXDRIVER_CREATE createDriver;
    SRGFXDRIVER_DESTROY destroyDriver;
    SRGFXDRIVER_LOCK lock;
    SRGFXDRIVER_UNLOCK unlock;
    SRGFXDRIVER_FLIP flip;
    SRGFXDRIVER_CLEAR clear;
    SRGFXDRIVER_CLEARALL clearAll;
    SRGFXDRIVER_SETVIEWPORT setViewPort;
    SRGFXDRIVER_SETBG setBackGround;
    u32 field_24;
    ColorBGRA field_28;
    u32 field_38;
    u32 field_3C;
    SRGFXDRIVER_INITPOLYGONSET initPolygonSet;
    SRGFXDRIVER_LOADGROUP loadGroup;
    SRGFXDRIVER_SETMATRIX setMatrix;
    SRGFXDRIVER_UNLOADTEXTURE unloadTexture;
    SRGFXDRIVER_LOADTEXTURE loadTexture;
    SRGFXDRIVER_PALETTECHANGED paletteChanged;
    SRGFXDRIVER_WRITEPALLETTE writePalette;
    SRGFXDRIVER_BLENDMODE blendmode;
    SRGFXDRIVER_LIGHTPOLYGONSET lightPolygonSet;
    SRGFXDRIVER_FIELD64CB field_64;
    SRGFXDRIVER_SETRENDERSTATE setRenderState;
    SRGFXDRIVER_SETRENDERSTATE _setRenderState;
    SRGFXDRIVER_SETRENDERSTATE __setRenderState;
    SRGFXDRIVER_FIELD74CB field_74;
    SRGFXDRIVER_FIELD78CB field_78;
    SRGFXDRIVER_DEFERREDDRAW deferredDraw;
    SRGFXDRIVER_FIELD80CB field_80;
    SRGFXDRIVER_FIELD84CB field_84;
    SRGFXDRIVER_BEGINSCENE beginScene;
    SRGFXDRIVER_ENDSCENE endScene;
    SRGFXDRIVER_FIELD90CB field_90;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_flat2D;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_smooth2D;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_textured2D;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_paletted2D;
    SRGFXDRIVER_SETPOLYRENDER _setrenderstate_paletted2D;
    SRGFXDRIVER_DRAWVERTICES drawFlat2D;
    SRGFXDRIVER_DRAWVERTICES drawSmooth2D;
    SRGFXDRIVER_DRAWVERTICES drawTextured2D;
    SRGFXDRIVER_DRAWVERTICES drawPaletted2D;
    SRGFXDRIVER_SETPOLYRENDER setRenderState_flat3D;
    SRGFXDRIVER_SETPOLYRENDER setRenderState_smooth3D;
    SRGFXDRIVER_SETPOLYRENDER setRenderState_textured3D;
    SRGFXDRIVER_SETPOLYRENDER setRenderState_paletted3D;
    SRGFXDRIVER_SETPOLYRENDER _setRenderState_paletted3D;
    SRGFXDRIVER_DRAWVERTICES drawFlat3D;
    SRGFXDRIVER_DRAWVERTICES drawSmooth3D;
    SRGFXDRIVER_DRAWVERTICES drawTextured3D;
    SRGFXDRIVER_DRAWVERTICES drawPaletted3D;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_flatlines;
    SRGFXDRIVER_SETPOLYRENDER setrenderstate_smoothlines;
    SRGFXDRIVER_DRAWVERTICES draw_flatlines;
    SRGFXDRIVER_DRAWVERTICES draw_smoothlines;
    SRGFXDRIVER_FIELDECCB field_EC;
};
#pragma pack(pop)

typedef enum {
    V_WIREFRAME,			// 0x1

    V_TEXTURE,				// 0x2

    V_LINEARFILTER,			// 0x4

    V_PERSPECTIVE,			// 0x8

    V_TMAPBLEND,			// 0x10

    V_WRAP_U,				// 0x20

    V_WRAP_V,				// 0x40

    V_UNKNOWN80,			// 0x80

    V_COLORKEY,				// 0x100

    V_DITHER,				// 0x200

    V_ALPHABLEND,			// 0x400

    V_ALPHATEST,			// 0x800

    V_ANTIALIAS,			// 0x1000

    V_CULLFACE,				// 0x2000

    V_NOCULL,				// 0x4000

    V_DEPTHTEST,			// 0x8000

    V_DEPTHMASK,			// 0x10000

    V_SHADEMODE,			// 0x20000

    V_SPECULAR,				// 0x40000
    V_LIGHTSTATE,			// 0x80000
    V_FOG,					// 0x100000
    V_TEXADDR,				// 0x200000
    V_UNKNOWN400000,        // 0x400000
    V_UNKNOWN800000,        // 0x800000
    V_ALPHAFUNC,            // 0x1000000
    V_ALPHAREF,             // 0x2000000
} GFX_EFFECTS;


typedef void*(*SRPFN_GAMEFREE)(void*, const char*, u32);
typedef void*(*SRPFN_GAMEMALLOC)(u32, const char*, u32);
typedef void*(*SRPFN_GAMEMALLOCARRAY)(u32, u32, const char*, u32);
typedef void(*SRPFN_DESTROYGFXDRIVER)(GraphicsDriverCallbacks*);
typedef GraphicsDriverCallbacks*(*SRPFN_CREATEGFXDRIVER)();
typedef TextureFormat*(*SRPFN_CREATETEXTUREFORMAT)();
typedef void(*SRPFN_LISTADDTEXTUREFORMAT)(TextureFormat*, GameContext*);
typedef GameContext* (*SRPFN_GETGAMECTX)();
typedef Palette*(*SRPFN_CREATETEXTUREPALETTE)(u32, TextureHeader*, TextureSet*);
typedef TextureSet*(*SRPFN_CREATETEXTURESET)();
typedef void(*SRPFN_DESTROYTEXTURESETPALETTE)(TextureSet*);
typedef void(*SRPFN_DESTROYPALETTE)(Palette*);
typedef Palette*(*SRPFN_CREATEPALETTE)(u32, u32, u32, u32, u32);
typedef void(*SRPFN_CONVERTOPIXELS)(u32, u32, u32, u32, u32, TextureFormat*);

#pragma pack(push, 1)
struct _GraphicsObjectExternals{
    SRPFN_GAMEFREE gameFree;
    SRPFN_GAMEMALLOC gameMalloc;
    SRPFN_GAMEMALLOCARRAY gameMallocArray;
    SRPFN_CREATETEXTUREFORMAT createTextureFormat;
    SRPFN_LISTADDTEXTUREFORMAT listAddTextureFormat;
    SRPFN_GETGAMECTX getGameContext;
    SRPFN_DESTROYGFXDRIVER destroyDriver;
    SRPFN_CREATEGFXDRIVER createGFXDriver;
    SRPFN_CONVERTOPIXELS makePixelFormat;
    void* gltexformat2texformat;
    void* sub_686143;
    void* sub_6861EC;
    void* sub_68631E;
    void* sub_686351;
    void* palPixelToBGRA;
    void* palPixel2BGRA8bit;
    SRPFN_DESTROYTEXTURESETPALETTE destroyTextureSetPAL;
    SRPFN_CREATETEXTUREPALETTE createTexturePalette;
    void* convertTexture;
    void* textureSetDecref;
    SRPFN_CREATETEXTURESET createTextureSet;
    void* copyPaletteColor;
    void* rgba2d3dCol;
    void* sub_6A5FEB;
    void* sub_6A604A;
    SRPFN_DESTROYPALETTE destroyPalette;
    SRPFN_CREATEPALETTE createPalette;
    void* writePalette;
    void* paletteChanged;
    void* sub_6A5A70;
    void* sub_6A5BA0;
    void* sub_6A5C3B;
    void* sub_6A5CE2;
    void* sub_6A2865;
    SRGFXDRIVER_LOADGROUP genericLoadGroup;
    SRGFXDRIVER_LIGHTPOLYGONSET genericLightPolygonSet;
};
#pragma pack(pop)

#endif
