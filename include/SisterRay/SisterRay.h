#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stddef.h>
#include <stdint.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>
#include <SisterRay/data_addresses.h>

/*Macro for handling compilation in both C and C++*/
#if defined(__cplusplus)
# define SISTERRAY_API extern "C"
#else
# define SISTERRAY_API
#endif

#if defined(__cplusplus)
# define SISTERRAY_GLOBAL extern "C"
#else
# define SISTERRAY_GLOBAL extern
#endif

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*x))

typedef enum {
    NO_TYPE,
    INIT_EQUIP_MENU,
    DRAW_EQUIP_MENU,
    EQUIP_MENU_INPUT_OK,
    EQUIP_MENU_INPUT_CANCEL,
    EQUIP_MENU_INPUT_SQUARE,
    EQUIP_MENU_INPUT_TRIANGLE,
    EQUIP_MENU_INPUT_R1,
    EQUIP_MENU_INPUT_L1
} SrEventType;

typedef u32 SrEventListener;
typedef void (*SrEventCallback)(const void*);

/* SisterRay API */
typedef SrEventListener (PFNSRADDLISTENERPROC)(SrEventType eventType, SrEventCallback callback);

typedef const void* (*PFNSRLOADFUNCTIONPROC)(const char*);
typedef const void* (*PFNSRREGISTERFUNCTIONPROC)(const char*, const void*);

/* Mog re-exports */
typedef void  (*PFNSRREPLACEFUNCTIONPROC)(void* dst, void* newAddr);
typedef void* (*PFNSRREDIRECTFUNCTIONPROC)(void* dst, void* newAddr);
typedef void  (*PFNSRREPLACESKIPPROC)(void* dst, size_t len);
typedef void  (*PFNSRREPLACENOPPROC)(void* dst, size_t len);
typedef void* (*PFNSRVIRTUALADDRESSPROC)(uint32_t fixedAddr);

#endif
