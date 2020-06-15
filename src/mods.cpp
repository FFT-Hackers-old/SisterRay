#include <windows.h>
#include <string.h>
#include <fstream>
#include <string>
#include "mods/character_mechanics_mod/char_commands.h"
#include "mods/new_spells.h"
#include "mods/controllable_summon.h"
#include "mods/stagger.h"

#include "impl.h"

typedef void (*PFNMODMAINPROC)(PFNSRLOADFUNCTIONPROC srLoadFunction);

static const char* ff7ModPath(const char* mod)
{
    static char buffer[MAX_PATH];
    strcpy_s(buffer, "mods\\");
    strcat_s(buffer, mod);
    return srGetGamePath(buffer);
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
    const char* modsListsPath = srGetGamePath("mods.txt");
    std::ifstream mods(modsListsPath);
    std::string modName;
    loadCharMod();
    registerEruption();
    loadPlayableSummons();
    loadStagger();

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
