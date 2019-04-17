#include "lua_api.h"

SISTERRAY_API void srInitLua(void)
{
    lua_State* L;
    L = luaL_newstate();
    gContext.L = L;
    luaL_openlibs(L);

    /* Init metatables */
    srLuaInitItem();

    /* Create gamedata */
    lua_newtable(L);
    srLuaInitItems();
    lua_setfield(L, -2, "items");
    lua_setglobal(L, "gamedata");
}
