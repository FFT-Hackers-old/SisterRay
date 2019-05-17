#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../battle/ai_scripts.h"
#include "party_utils.h"

typedef struct {
    BattleAIData characterAI;
} SrCharacterData;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrCharacterRegistry : public SrNamedResourceRegistry<SrCharacterData, std::string> {
public:
    SrCharacterRegistry() : SrNamedResourceRegistry<SrCharacterData, std::string>() {}
};

SISTERRAY_API void initCharacterData(SrKernelStream* stream);
#endif
