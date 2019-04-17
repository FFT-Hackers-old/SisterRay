#include <stdio.h>
#include "lua_api.h"

static int itemName(lua_State* L)
{
    u16 itemID;

    itemID = *((u16*)lua_touserdata(L, 1));
    lua_pushstring(L, gContext.game_strings.item_names.get_unicode_string(itemID));
    return 1;
}

static int itemSetName(lua_State* L)
{
    u16 itemID;

    itemID = *((u16*)lua_touserdata(L, 1));
    gContext.game_strings.item_names.set_unicode_string(itemID, luaL_checkstring(L, 3));
    return 0;
}

static int itemToString(lua_State* L)
{
    char buffer[256];
    u16 itemID;

    itemID = *((u16*)lua_touserdata(L, 1));
    snprintf(buffer, 256, "<Item #%04x>", (int)itemID);
    lua_pushstring(L, buffer);
    return 1;
}

static int itemIndex(lua_State* L)
{
    const char* str;

    str = luaL_checkstring(L, 2);
    if (strcmp(str, "name") == 0)
        return itemName(L);
    return 0;
}

static int itemNewindex(lua_State* L)
{
    const char* str;

    str = luaL_checkstring(L, 2);
    if (strcmp(str, "name") == 0)
        return itemSetName(L);
    return 0;
}

void srLuaInitItem(void)
{
    lua_State* L = gContext.L;

    luaL_newmetatable(L, "SisterRay.item");
    lua_pushcfunction(L, itemToString);
    lua_setfield(L, -2, "__tostring");
    lua_pushcfunction(L, itemIndex);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, itemNewindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}

void srLuaGetItem(u16 itemID)
{
    u16* ptr;
    lua_State* L = gContext.L;

    ptr = (u16*)lua_newuserdata(L, 2);
    *ptr = itemID;
    luaL_getmetatable(L, "SisterRay.item");
    lua_setmetatable(L, -2);
}
