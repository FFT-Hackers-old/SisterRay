#ifndef WIDGET_H
#define WIDGET_H

#include "primitives.h"
#include "assets.h"
#include "containers.h"
#include "updaters.h"

/*The Widget type and its subtypes need to be capable of C interop for stability crossing DLL boundaries, to ensure
  a typesafe API for event dispatches and authoring callbacks on those dispatches, any types contained in the dispatch
  should be C types. Ergo Widget, Menu, etc types are implmented in C and are managed internal registries to prevent memory leaks
  For the above reasons, It is not adviseable to attempt to implement these features using modern C++ constructs*/

#endif
