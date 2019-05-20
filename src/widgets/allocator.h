#ifndef CONTAINERS_TABLE_H
#define CONTAINERS_TABLE_H

#include "widget.h"

Widget* typeAllocate(const WidgetClass* type, std::string name, void* initParams=nullptr);

#endif
