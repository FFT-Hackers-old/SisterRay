#include "lua_api.h"

static int itemsIndex(lua_State* L)
{
    u16 itemID = luaL_checknumber(L, 2);
    srLuaGetItem(itemID);
    return 1;
}

void srLuaInitItems(void)
{
    lua_State* L = gContext.L;

    lua_newtable(L);

    luaL_newmetatable(L, "SisterRay.items");
    lua_pushcfunction(L, itemsIndex);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
}
