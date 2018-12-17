#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>

/*Macro for handling compilation in both C and C++*/
#if defined(__cplusplus)
# define SISTERRAY_API extern "C"
#else
# define SISTERRAY_API
#endif

#endif
