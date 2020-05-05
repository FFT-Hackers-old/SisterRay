#ifndef SUMMON_H
#define SUMMON_H

#include <SisterRay/SisterRay.h>
#include <string>
#include <vector>


#define SUMMON_MOD_NAME  "srSummonBase"
const std::vector<std::string> summonModelNames = {
    std::string("LIMIT/MOGURIDA.DAT"),
    std::string("LIMIT/CYVADAT.DAT"),
    std::string("LIMIT/IFREETDA.DAT"),
    std::string("LIMIT/LAMDAT.DAT"),
    std::string("LIMIT/TITANDAT.DAT"),
    std::string("LIMIT/ODIN_ZDA.DAT"),
    std::string("LIMIT/RIVADAT.DAT"),
    std::string("LIMIT/BAHAMDAT.DAT"),
    std::string("LIMIT/KUJATADA.DAT"),
    std::string("LIMIT/ALEXDAT.DAT"),
    std::string("LIMIT/PHEONIXD.DAT"),
    std::string("LIMIT/BAHAMRDA.DAT"),
    std::string("LIMIT/HADESDAT.DAT"),
    std::string("LIMIT/TUPONDAT.DAT"),
    std::string("LIMIT/BAHAMZDA.DAT"),
    std::string("LIMIT/DEBCYODA.DAT"),
    std::string("LIMIT/ODIN_GDA.DAT")
};

static const std::vector<std::string> summonLGPSeekNames = {
    std::string("MOGURIDA.DAT"),
    std::string("CYVADAT.DAT"),
    std::string("IFREETDA.DAT"),
    std::string("LAMDAT.DAT"),
    std::string("TITANDAT.DAT"),
    std::string("ODIN_ZDA.DAT"),
    std::string("RIVADAT.DAT"),
    std::string("BAHAMDAT.DAT"),
    std::string("KUJATADA.DAT"),
    std::string("ALEXDAT.DAT"),
    std::string("PHEONIXD.DAT"),
    std::string("BAHAMRDA.DAT"),
    std::string("HADESDAT.DAT"),
    std::string("TUPONDAT.DAT"),
    std::string("BAHAMZDA.DAT"),
    std::string("DEBCYODA.DAT"),
    std::string("ODIN_GDA.DAT")
};

void initSummons(u8* magicLGP);
void loadSummonAnimScripts();
void loadSummonAnimations(u8* magicLGPBuffer);
#endif // !LIMIT_H
