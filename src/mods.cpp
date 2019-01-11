#include <windows.h>
#include <string.h>
#include <fstream>
#include <string>

#include "impl.h"

typedef void (*PFNMODMAINPROC)(PFNSRLOADFUNCTIONPROC srLoadFunction);

static const char* ff7Path(const char* suffix)
{
    size_t len;
    static char buffer[MAX_PATH];

    memset(buffer, 0, MAX_PATH);
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    len = strlen(buffer);
    for (size_t i = len - 1; i > 0; --i)
    {
        if (buffer[i] == 0)
            break;
        if (buffer[i] == '/' || buffer[i] == '\\')
        {
            buffer[i] = 0;
            break;
        }
    }
    strcat_s(buffer, "\\");
    strcat_s(buffer, suffix);

    return buffer;
}

static const char* ff7ModPath(const char* mod)
{
    static char buffer[MAX_PATH];
    strcpy_s(buffer, "mods\\");
    strcat_s(buffer, mod);
    return ff7Path(buffer);
}

static void LoadMod(const char* name)
{
    const char* path = ff7ModPath(name);
    HMODULE mod = LoadLibraryA(path);
    PFNMODMAINPROC modMain = NULL;

    if (mod)
    {
        modMain = (PFNMODMAINPROC)GetProcAddress(mod, "ModMain");
    }

    if (modMain)
    {
        modMain(&srLoadFunction);
    }
}

SISTERRAY_API void LoadMods(void)
{
    const char* modsListsPath = ff7Path("mods.txt");
    std::ifstream mods(modsListsPath);
    std::string modName;

    if (!mods.good())
        return;
    while (mods.good())
    {
        modName = "";
        std::getline(mods, modName);
        if (modName[0] == '#')
            continue;
        if (modName.size() == 0)
            continue;
        if (modName[modName.size() - 1] == '\r')
            modName.pop_back();
        if (modName.size() == 0)
            continue;
        LoadMod(modName.c_str());
    }
}