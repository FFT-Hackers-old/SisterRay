#ifndef STATUSES_H
#define STATUSES_H

#include <string>
#include "../EncodedString.h"
#include "../sr_named_registry.h"

typedef struct {
    EncodedString displayName;
    bool isGameStatus;
    bool gameIndex;
    std::string penName;
    std::string resName;
    bool persistsAfterBattle;
    std::vector<std::string> removeOnInflict;
    std::vector<std::string> neutralizeOnInflict;
}SrStatus;

typedef struct {
    u8 inflictionChance;
    bool doesToggle;
    bool doesRemove;
    u16 duration;
}StatusInfliction;

class SrStatusRegistry : public SrNamedResourceRegistry<SrStatus, std::string> {
public:
    SrStatusRegistry() : SrNamedResourceRegistry<SrStatus, std::string>() {}
    SrStatusRegistry(bool useResistances = true);
};

void initStatuses();
std::string getStatusIDFromIndex(u8 statusIdx);

#endif 
