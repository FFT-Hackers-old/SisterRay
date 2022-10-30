#ifndef STATUSES_H
#define STATUSES_H

#include <string>
#include "../EncodedString.h"
#include "../sr_named_registry.h"

typedef struct {
    EncodedString displayName;
    bool isGameStatus;
    u8 gameIndex;
    std::string penName;
    std::string resName;
    SrStatusBehavior behavior;
    std::vector<std::string> removeOnInflict;
    std::vector<std::string> neutralizeOnInflict;
} SrStatus;

typedef struct {
    std::string statusName;
    u8 inflictionChance;
    bool doesToggle;
    bool doesRemove;
    u16 duration;
}StatusInfliction;

class SrStatusRegistry : public SrNamedResourceRegistry<SrStatus, std::string> {
public:
    SrStatusRegistry() : SrNamedResourceRegistry<SrStatus, std::string>() {}
    explicit SrStatusRegistry(bool useResistances);
};

void initStatuses();
std::string getStatusIDFromIndex(u8 statusIdx);

#endif 
