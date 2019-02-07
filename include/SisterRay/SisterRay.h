#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stddef.h>
#include <stdint.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>

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

/* SisterRay API */
typedef const void* (*PFNSRLOADFUNCTIONPROC)(const char*);
typedef const void* (*PFNSRREGISTERFUNCTIONPROC)(const char*, const void*);

/* Mog re-exports */
typedef void  (*PFNSRREPLACEFUNCTIONPROC)(void* dst, void* newAddr);
typedef void* (*PFNSRREDIRECTFUNCTIONPROC)(void* dst, void* newAddr);
typedef void  (*PFNSRREPLACESKIPPROC)(void* dst, size_t len);
typedef void  (*PFNSRREPLACENOPPROC)(void* dst, size_t len);
typedef void* (*PFNSRVIRTUALADDRESSPROC)(uint32_t fixedAddr);

#endif
