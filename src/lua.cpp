#include "impl.h"

SISTERRAY_API void srInitLua(void)
{
    gContext.L = luaL_newstate();
    luaL_openlibs(gContext.L);
}
