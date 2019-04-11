#ifndef SISTERRAY_API_H
#define SISTERRAY_API_H

#include <SisterRay/SisterRay.h>

/*
 * This header contains the internal signatures of every
 * API function.
 * It's used to export these functions to the registry
 * for mod consumption.
 */

SISTERRAY_API SrEventListener srAddListener(SrEventType eventType, SrEventCallback callback);

#endif
